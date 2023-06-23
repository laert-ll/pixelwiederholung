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
    uint8_t* img = malloc(width * height * sizeof(uint8_t));

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
    printf("Total pixel array size: %u\n", width*height);

    // Einlesen des Pixel-Arrays in das img-Array
    fseek(file, offset, SEEK_SET);
    fread(img, sizeof(uint8_t), width*height, file);

    // Das Bild zurückschreiben
    write_image(header, img, width, height, output);

    fclose(file);
    free(img);
}

void write_image(uint8_t* header, const uint8_t* img, uint32_t width, uint32_t height, const char* output) {

    // Neue Datei erstellen
    FILE* newfile = fopen(output, "wb");

    // Header und das Pixel-Array hineinschreiben
    fwrite(header, sizeof(uint8_t), 54, newfile);
    fwrite(img, sizeof(uint8_t), width*height, newfile);

    fclose(newfile);
}
