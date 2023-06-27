#include "bmp_io.h"
#include "img_proc.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

void read_image(const char *input, const char *output) {

    FILE *file = fopen(input, "rb");

    uint32_t offset;
    uint32_t width = 0;
    uint32_t height = 0;
    uint8_t header[54];
    uint8_t *img = malloc(width * height * sizeof(uint8_t));

    // Header speichern
    fread(header, sizeof(uint8_t), 54, file);

    // Anfang der Pixeldaten extrahieren
    fseek(file, 10, SEEK_SET);
    fread(&offset, sizeof(uint8_t), 4, file);

    // Breite extrahieren
    fseek(file, 18, SEEK_SET);
    fread(&width, sizeof(uint8_t), 4, file);

    // Höhe extrahieren
    fseek(file, 22, SEEK_SET);
    fread(&height, sizeof(uint8_t), 4, file);

    // Bildinformation printen
    printf("Offset: %u\n", offset);
    printf("Image Width: %u\n", width);
    printf("Image Height: %u\n", height);

    // Berechnen der Breite in Pixeln + Padding
    width = ((width * 3) + (width % 4));

    printf("Image Width in bytes: %u\n", width);
    printf("Image Height in pixels: %u\n", height);
    printf("Total pixel array size: %u\n", width * height);

    // Einlesen des Pixel-Arrays in das img-Array
    fseek(file, offset, SEEK_SET);
    fread(img, sizeof(uint8_t), width * height, file);

    // Das Bild zurückschreiben
    write_image(header, img, width, height, output);

    fclose(file);
    free(img);
}

void write_image(uint8_t *header, const uint8_t *img, uint32_t width, uint32_t height, const char *output) {

    // Neue Datei erstellen
    FILE *newfile = fopen(output, "wb");

    // Header und das Pixel-Array hineinschreiben
    fwrite(header, sizeof(uint8_t), 54, newfile);
    fwrite(img, sizeof(uint8_t), width * height * 3, newfile);

    fclose(newfile);
}

void window_test(const char *input, const char *output, size_t x, size_t y, size_t width, size_t height) {

    FILE *file = fopen(input, "rb");
    
    uint8_t header[54];
    uint32_t offset, imageWidth, imageHeight;

    // Header speichern
    fread(header, sizeof(uint8_t), 54, file);
    // Anfang der Pixeldaten extrahieren
    fseek(file, 10, SEEK_SET);
    fread(&offset, sizeof(uint8_t), 4, file);
    // Breite extrahieren
    fseek(file, 18, SEEK_SET);
    fread(&imageWidth, sizeof(uint8_t), 4, file);
    // Höhe extrahieren
    fseek(file, 22, SEEK_SET);
    fread(&imageHeight, sizeof(uint8_t), 4, file);

    // Berechnen der Breite in Pixeln + Padding
    imageWidth = ((imageWidth * 3) + (imageWidth % 4));

    // Einlesen des Pixel-Arrays in das img-Array
    uint8_t *img = malloc(imageWidth * imageHeight * sizeof(uint8_t));
    fseek(file, offset, SEEK_SET);
    fread(img, sizeof(uint8_t), imageWidth * imageHeight, file);
    fclose(file);

    // window aufrufen
    uint8_t *windowImg = malloc(width * height * sizeof(uint8_t) * 3);
    window(img, x, y, width, height, windowImg, imageWidth, imageHeight);

    // Header modifizieren
    memcpy(header + 18, &width, sizeof(uint32_t));
    memcpy(header + 22, &height, sizeof(uint32_t));

    // Write the new image
    write_image(header, windowImg, width, height, output);

    free(img);
    free(windowImg);
}
