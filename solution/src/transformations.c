#include "transformations.h"
#include "image.h"
#include <stdint.h>

struct image* rotate(const struct image* source) {
    struct image* img = createImage(source->height, source->width);

    for (uint64_t y = 0; y < source->height; y++) {
        for(uint64_t x = 0; x < source->width; x++) {
            setPixel(img, y, source->width - x - 1, getPixel(source, x, y));
        }
    }

    return img;
}
