#include "image.h"
#include <stdint.h>

struct image* createImage(uint64_t width, uint64_t height) {
    struct image* img = (struct image*) malloc(sizeof(struct image));
    img->width = width;
    img->height = height;

    img->data = (struct pixel*) malloc(sizeof(struct pixel) * width * height);

    return img;
}

void resizeImage(struct image* img, uint64_t width, uint64_t height) {
    struct pixel* newData = (struct pixel*) malloc(sizeof(struct pixel) * width * height);

    uint64_t minWidth = (img->width < width ? img->width : width);
    uint64_t minHeight = (img->height < height ? img->height : height);

    for (uint64_t y = 0; y < minHeight; y++) {
        for (uint64_t x = 0; x < minWidth; x++) {
            newData[y*minWidth + x] = getPixel(img, x, y);
        }
    }

    free(img->data);
    img->data = newData;
    img->width = width;
    img->height = height;
}

void destroyImage(struct image* img) {
    free(img->data);
    free(img);
}

struct pixel getPixel(const struct image* img, uint64_t x, uint64_t y) {
    return img->data[y*img->width + x];
}

void setPixel(struct image* img, uint64_t x, uint64_t y, const struct pixel pixel) {
    img->data[y*img->width + x].r = pixel.r;
    img->data[y*img->width + x].g = pixel.g;
    img->data[y*img->width + x].b = pixel.b;
}
