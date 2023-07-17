#include "bmp_io.h"
#include "img_proc.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>

int pixelwiederholung(const char *input, const char *output, size_t x, size_t y, size_t width, size_t height,
                       size_t scale_factor, int version) {

    if (version != 0) {
        printf("Nur Version 0 möglich.\n");
        return -1;
    }

    FILE *file = fopen(input, "rb");
    if (file == NULL) {
        perror("Failed to open the input file");
        return -1;
    }

    uint8_t header[54];
    uint32_t offset, imageWidth, imageHeight;

    // Header speichern
    if (fread(header, sizeof(uint8_t), 54, file) != 54) {
        perror("Failed to read the header");
        fclose(file);
        return -1;
    }

    // Anfang der Pixeldaten extrahieren
    fseek(file, 10, SEEK_SET);
    if (fread(&offset, sizeof(uint8_t), 4, file) != 4) {
        perror("Failed to read the offset");
        fclose(file);
        return -1;
    }

    // Breite extrahieren
    fseek(file, 18, SEEK_SET);
    if (fread(&imageWidth, sizeof(uint8_t), 4, file) != 4) {
        perror("Failed to read the image width");
        fclose(file);
        return -1;
    }

    // Höhe extrahieren
    fseek(file, 22, SEEK_SET);
    if (fread(&imageHeight, sizeof(uint8_t), 4, file) != 4) {
        perror("Failed to read the image height");
        fclose(file);
        return -1;
    }

    // ungültige Argumente für x,y,width,heigth abfangen  
    if (x < 0 || y < 0 ||
        width <= 0 || height <= 0 ||
        x + width > imageWidth ||
        y + height > imageHeight) {
        printf("Außerhalb des gültiges Wertebereichs.\n");
        return -1;
    }

    // Berechnen der Breite in Pixeln + Padding
    imageWidth = ((imageWidth * 3) + (imageWidth % 4));

    // Einlesen des Pixel-Arrays in das img-Array
    uint8_t *img = malloc(imageWidth * imageHeight * sizeof(uint8_t));
    fseek(file, offset, SEEK_SET);
    if (fread(img, sizeof(uint8_t), imageWidth * imageHeight, file) != imageWidth * imageHeight) {
        perror("Failed to read the pixel data");
        free(img);
        fclose(file);
        return -1;
    }
    fclose(file);

    // window aufrufen
    uint8_t *windowImg = malloc(((width * 3) + (width % 4)) * height * sizeof(uint8_t));
    window(img, x, y, width, height, windowImg, imageWidth, imageHeight);

    // zoom aufrufen
    size_t zoomedSize =
            ((width * scale_factor * 3) + (width * scale_factor % 4)) * imageHeight * scale_factor * sizeof(uint8_t);
    uint8_t *zoomImg = malloc(zoomedSize);
    memset(zoomImg, 0, zoomedSize);
    zoom(windowImg, width, height, scale_factor, zoomImg, version);

    // zoomed width & height richtig setzen
    width = width * scale_factor;
    height = height * scale_factor;

    // Header modifizieren
    memcpy(header + 18, &width, sizeof(uint32_t));
    memcpy(header + 22, &height, sizeof(uint32_t));

    // Neue Datei erstellen
    FILE *newfile = fopen(output, "wb");

    // Header und das Pixel-Array hineinschreiben
    fwrite(header, sizeof(uint8_t), 54, newfile);
    fwrite(zoomImg, sizeof(uint8_t), ((width * 3) + (width % 4)) * height, newfile);

    fclose(newfile);
    free(img);
    free(windowImg);
    return 0;

}
