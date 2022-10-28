#include "bmp.h"
#include "image.h"
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#define HEADER_TYPE 19778
#define HEADER_RESERVED 0
#define HEADER_OFFBITS 54
#define HEADER_SIZE  40
#define HEADER_PLANES 1
#define HEADER_BIT_COUNT 24
#define HEADER_COMPRESSION 0
#define HEADER_PIXELS_PER_METER 2834
#define HEADER_COLORS_USED 0
#define HEADER_COLORS_IMPORTANT 0

static inline size_t getPaddingSize(size_t width) {
    return 4 - (3 * width % 4) % 4;
}

enum readStatus fromBmp(FILE* in, struct image* img) {
    struct bmpHeader header = {0};
    size_t count = fread(&header, sizeof(struct bmpHeader), 1, in);

    if (count != 1) return READ_INVALID_HEADER;
    
    if (header.bfType != HEADER_TYPE) return READ_INVALID_SIGNATURE;

    if (header.biBitCount != HEADER_BIT_COUNT) return READ_INVALID_BIT_COUNT;

    *img = createImage(header.biWidth, header.biHeight);
    size_t paddingSize = getPaddingSize(header.biWidth);

    // BMP scan lines are stored from bottom to top
    for (size_t _y = img->height; _y > 0; _y--) {
        // Workaround
        // We want to loop y in the range of [img->height-1, 0]
        // We can't check for y < 0, because size_t is never negative.
        // Also not y == 0, since we will lose a cycle for y == 0.
        size_t y = _y - 1;

        size_t count = fread(img->data + y * img->width, sizeof(struct pixel), img->width, in);
        if (count < img->width) {
            return READ_INVALID_BITS;
        }

        // BMP pixels RGB values are stored backwards
        for (size_t x = 0; x < img->width; x++) {
            struct pixel p = getPixel(img, x, y);
            uint8_t temp = p.r;
            p.r = p.b;
            p.b = temp;
            setPixel(img, x, y, p);
        }
        // Skip padding
        if (fseek(in, paddingSize, SEEK_CUR) != 0) {
            return READ_INVALID_BITS;
        }
    }

    return READ_OK;
}

enum writeStatus toBmp(FILE* out, const struct image* img) {
    uint64_t paddingSize = getPaddingSize(img->width);
    uint64_t lineSize = sizeof(uint8_t) * (img->width * 3 + paddingSize);

    struct bmpHeader header = {0};
    header.bfType = HEADER_TYPE;
    header.biSizeImage = (img->width*3 + paddingSize) * img->height;
    header.bfileSize = header.biSizeImage + sizeof(struct bmpHeader);
    header.bfReserved = HEADER_RESERVED;
    header.bOffBits = HEADER_OFFBITS;
    header.biSize = HEADER_SIZE;
    header.biWidth = img->width;
    header.biHeight = img->height;
    header.biPlanes = HEADER_PLANES;
    header.biBitCount = HEADER_BIT_COUNT;
    header.biCompression = HEADER_COMPRESSION;

    header.biXPelsPerMeter = HEADER_PIXELS_PER_METER;
    header.biYPelsPerMeter = HEADER_PIXELS_PER_METER;
    header.biClrUsed = HEADER_COLORS_USED;
    header.biClrImportant = HEADER_COLORS_IMPORTANT;

    size_t count = fwrite(&header, sizeof(struct bmpHeader), 1, out);

    if (count < 1) {
        return WRITE_ERROR;
    }

    for (size_t _y = img->height; _y > 0; _y--) {
        size_t y = _y - 1;

        uint8_t* scanLine = malloc(lineSize);

        // Make BGR
        for (size_t x = 0; x < img->width; x++) {
            struct pixel pixel = getPixel(img, x, y);
            scanLine[3*x] = pixel.b;
            scanLine[3*x+1] = pixel.g;
            scanLine[3*x+2] = pixel.r;
        }

        // Zero padding
        for (size_t x = 0; x < paddingSize; x++) {
            scanLine[img->width*3 + x] = 0;
        }

        size_t count = fwrite(scanLine, sizeof(uint8_t), img->width*3+paddingSize, out);
        free(scanLine);

        if (count < img->width + paddingSize) {
            return WRITE_ERROR;
        }

        if (y == 0) break;
    }

    return WRITE_OK;
}
