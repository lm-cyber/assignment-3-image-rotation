#include "bmp.h"
#include "image.h"
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

enum readStatus fromBmp(FILE* in, struct image* img) {
    struct bmpHeader header;
    size_t count = fread(&header, sizeof(struct bmpHeader), 1, in);

    if (count != 1) return READ_INVALID_HEADER;
    
    if (header.bfType != 'M' * 256 + 'B') return READ_INVALID_SIGNATURE;

    if (header.biBitCount != 24) return READ_INVALID_BIT_COUNT;

    resizeImage(img, header.biWidth, header.biHeight);
    uint64_t paddingSize = (4 - (3 * img->width % 4)) % 4;

    // BMP scan lines are stored from bottom to top
    for (uint64_t y = img->height-1;; y--) {
        size_t count = fread(img->data + y * img->width, sizeof(struct pixel), img->width, in);
        if (count < img->width) {
            return READ_INVALID_BITS;
        }
        // BMP pixels RGB values are stored backwards
        for (int x = 0; x < img->width; x++) {
            struct pixel p = getPixel(img, x, y);
            uint8_t temp = p.r;
            p.r = p.b;
            p.b = temp;
            setPixel(img, x, y, p);
        }
        // Skip padding
        fseek(in, paddingSize, SEEK_CUR);

        if (y == 0) break;
    }

    return READ_OK;
}

enum writeStatus toBmp(FILE* out, const struct image* img) {
    uint64_t paddingSize = (4 - (3 * img->width % 4)) % 4;
    uint64_t lineSize = sizeof(uint8_t) * (img->width * 3 + paddingSize);

    struct bmpHeader header;
    header.bfType = 'M' * 256 + 'B';
    header.biSizeImage = (img->width*3 + paddingSize) * img->height;
    header.bfileSize = header.biSizeImage + sizeof(struct bmpHeader);
    header.bfReserved = 0;
    header.bOffBits = 54;
    header.biSize = 40;
    header.biWidth = img->width;
    header.biHeight = img->height;
    header.biPlanes = 1;
    header.biBitCount = 24;
    header.biCompression = 0;

    header.biXPelsPerMeter = 2834;
    header.biYPelsPerMeter = 2834;
    header.biClrUsed = 0;
    header.biClrImportant = 0;

    size_t count = fwrite(&header, sizeof(struct bmpHeader), 1, out);

    if (count < 1) {
        return WRITE_ERROR;
    }

    for (uint64_t y = img->height-1;; y--) {
        uint8_t* scanLine = (uint8_t*) malloc(lineSize);

        // Make BGR
        for (uint64_t x = 0; x < img->width; x++) {
            scanLine[3*x] = getPixel(img, x, y).b;
            scanLine[3*x+1] = getPixel(img, x, y).g;
            scanLine[3*x+2] = getPixel(img, x, y).r;
        }

        // Zero padding
        for (uint64_t x = 0; x < paddingSize; x++) {
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
