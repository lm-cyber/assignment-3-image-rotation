#ifndef IMAGE_HEADER
#define IMAGE_HEADER

#include <stdint.h>
#include <stdlib.h>

struct pixel {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

struct image {
    uint64_t width, height;
    struct pixel* data;
};

struct image* createImage(uint64_t width, uint64_t height);

void resizeImage(struct image* img, uint64_t width, uint64_t height);

void destroyImage(struct image* img);

struct pixel getPixel(const struct image* img, uint64_t x, uint64_t y);

void setPixel(struct image* img, uint64_t x, uint64_t y, struct pixel pixel);
#endif
