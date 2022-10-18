#ifndef BMP_HEADER
#define BMP_HEADER
#include <stdint.h>
#include <stdio.h>
#include "image.h"

struct __attribute__((packed)) bmpHeader {
    uint16_t bfType;
    uint32_t bfileSize;
    uint32_t bfReserved;
    uint32_t bOffBits;
    uint32_t biSize;
    uint32_t biWidth;
    uint32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    uint32_t biXPelsPerMeter;
    uint32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
};

enum readStatus {
    READ_OK,
    READ_INVALID_SIGNATURE,
    READ_INVALID_BITS,
    READ_INVALID_HEADER,
    READ_INVALID_BIT_COUNT
};

enum readStatus fromBmp(FILE* in, struct image* img);

enum writeStatus {
    WRITE_OK,
    WRITE_ERROR
};

enum writeStatus toBmp(FILE* out, const struct image* img);
#endif
