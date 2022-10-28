#ifndef IMAGE_HEADER
#define IMAGE_HEADER

#include <stdint.h>
#include <stdlib.h>

struct __attribute__((packed)) pixel {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

struct image {
    size_t width, height;
    struct pixel* data;
};

struct image createImage(size_t width, size_t height);

void destroyImage(struct image* img);

struct pixel getPixel(const struct image* img, size_t x, size_t y);

void setPixel(struct image* img, size_t x, size_t y, struct pixel pixel);
#endif
