#include "image.h"
#include <stdint.h>

struct image createImage(size_t width, size_t height) {
    struct image img = {
        .width = width,
        .height = height,
        .data = malloc(sizeof(struct pixel) * width * height)
    };

    return img;
}

void destroyImage(struct image* img) {
    free(img->data);
}

struct pixel getPixel(const struct image* img, size_t x, size_t y) {
    return img->data[y*img->width + x];
}

void setPixel(struct image* img, size_t x, size_t y, const struct pixel pixel) {
    img->data[y*img->width+x] = pixel;
}
