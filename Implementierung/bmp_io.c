#include "bmp_io.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

void read_image(const char* input, const char* output) {

    FILE* file = fopen(input, "rb");

    uint32_t offset;
    uint32_t width;
    uint32_t height;
    uint8_t header[54];

    fread(header, sizeof(uint8_t), 54, file);

    fseek(file, 10, SEEK_SET);
    fread(&offset, sizeof(uint8_t), 4, file);

    fseek(file, 18, SEEK_SET);
    fread(&width, sizeof(uint8_t), 4, file);

    fseek(file, 22, SEEK_SET);
    fread(&height, sizeof(uint8_t), 4, file);

    printf("Offset: %u\n", offset);
    printf("Image Width: %u\n", width);
    printf("Image Height: %u\n", height);

    width = ((width * 3) + (width % 4));

    printf("Image Width in bytes: %u\n", width);
    printf("Image Height in pixels: %u\n", height);

    uint8_t* img = malloc(width * height * sizeof(uint8_t));

    fseek(file, offset, SEEK_SET);
    fread(img, sizeof(uint8_t), width*height, file);

    FILE* newfile = fopen(output, "wb");

    fwrite(header, sizeof(uint8_t), 54, newfile);
    fwrite(img, sizeof(uint8_t), width*height, newfile);

    fclose(newfile);
    fclose(file);
    free(img);
}
