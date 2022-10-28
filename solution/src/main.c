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

        if (in == NULL) {
            printf("Failed to open source file\n");
            return 1;
        }

        struct image img = {0};
        enum readStatus rs = fromBmp(in, &img);

        switch (rs) {
            case READ_OK:
                printf("File loaded\n");
                printf("Width: %" PRIu64 "\nHeight: %" PRIu64 "\n", img.width, img.height);
                break;
            case READ_INVALID_SIGNATURE:
                printf("Failed to load image: Invalid signature\n");
                return 1;
            case READ_INVALID_HEADER:
                printf("Failed to load image: Invalid header\n");
                return 1;
            case READ_INVALID_BITS:
                printf("Failed to load image: Invalid bits\n");
                return 1;
            case READ_INVALID_BIT_COUNT:
                printf("Failed to load image: Bit count not supported\n");
                return 1;
        }

        if (fclose(in) != 0) {
            printf("Failed to close source file\n");
            return 1;
        }

        struct image rotated = rotate(&img);
        destroyImage(&img);

        FILE* out = fopen(argv[2], "wb");

        if (out == NULL) {
            printf("Failed to open target file\n");
            return 1;
        }

        enum writeStatus ws = toBmp(out, &rotated);
        
        switch (ws) {
            case WRITE_OK:
                printf("Image saved\n");
                break;
            case WRITE_ERROR:
                printf("Failed to save image\n");
                break;
        }

        destroyImage(&rotated);
        
        if (fclose(out) != 0) {
            printf("Failed to close target file\n");
            return 1;
        }
    }
    
    return 0;
}
