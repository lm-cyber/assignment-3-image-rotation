#include "bmp.h"
#include "image.h"
#include "transformations.h"
#include <inttypes.h>
#include <stdio.h>

int main(int argc, char** argv) {
    if (argc < 3) {
        printf("Not enough arguments\n");
    } else {
        FILE* in = fopen(argv[1], "rb");
        struct image* img = createImage(0, 0);
        enum readStatus rs = fromBmp(in, img);

        switch (rs) {
            case READ_OK:
                printf("File loaded\n");
                printf("Width: %" PRIu64 "\nHeight: %" PRIu64 "\n", img->width, img->height);
                break;
            case READ_INVALID_SIGNATURE:
                printf("Failed to load image: Invalid signature\n");
                break;
            case READ_INVALID_HEADER:
                printf("Failed to load image: Invalid header\n");
                break;
            case READ_INVALID_BITS:
                printf("Failed to load image: Invalid bits\n");
                break;
            case READ_INVALID_BIT_COUNT:
                printf("Failed to load image: Bit count not supported\n");
                break;
        }

        fclose(in);

        struct image* rotated = rotate(img);
        destroyImage(img);

        FILE* out = fopen(argv[2], "wb");
        enum writeStatus ws = toBmp(out, rotated);
        
        switch (ws) {
            case WRITE_OK:
                printf("Image saved\n");
                break;
            case WRITE_ERROR:
                printf("Failed to save image\n");
                break;
        }

        destroyImage(rotated);
        fclose(out);
    }
    
    return 0;
}
