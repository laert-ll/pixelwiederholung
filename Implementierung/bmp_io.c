#include "bmp_io.h"
#include "img_proc.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

void window_test(const char *input, const char *output, size_t x, size_t y, size_t width, size_t height, size_t scale) {

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
    uint8_t *windowImg = malloc(((width * 3) + (width % 4)) * height * sizeof(uint8_t));
    window(img, x, y, width, height, windowImg, imageWidth, imageHeight);
    //    zoom(img, width, height, scale, input);
    // Header modifizieren
    memcpy(header + 18, &width, sizeof(uint32_t));
    memcpy(header + 22, &height, sizeof(uint32_t));

    // Neue image schreiben
    write_image(header, windowImg, width, height, output);

    free(img);
    free(windowImg);
}

void write_image(uint8_t *header, const uint8_t *img, uint32_t width, uint32_t height, const char *output) {

    // Neue Datei erstellen
    FILE *newfile = fopen(output, "wb");

    // Header und das Pixel-Array hineinschreiben
    fwrite(header, sizeof(uint8_t), 54, newfile);
    fwrite(img, sizeof(uint8_t), ((width * 3) + (width % 4)) * height, newfile);

    fclose(newfile);
}