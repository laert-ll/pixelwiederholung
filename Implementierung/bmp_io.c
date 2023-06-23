#include "bmp_io.h"
#include <stdio.h>
#include <stdint.h>

void read_header(const char* filepath) {

    FILE* file = fopen(filepath, "rb");

    if (file == NULL) {
        fprintf(stderr, "Failed to open the file: %s\n", filepath);
        return;
    }

    uint32_t offset;
    uint32_t width;
    uint32_t height;

    fseek(file, 10, SEEK_SET);
    fread(&offset, sizeof(uint8_t), 4, file);

    fseek(file, 18, SEEK_SET);
    fread(&width, sizeof(uint8_t), 4, file);

    fseek(file, 22, SEEK_SET);
    fread(&height, sizeof(uint8_t), 4, file);

    printf("Offset: %u\n", offset);
    printf("Image Width: %u\n", width);
    printf("Image Height: %u\n", height);

    fclose(file);
}
