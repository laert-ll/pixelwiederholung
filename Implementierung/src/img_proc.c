#include "bmp_io.h"
#include "img_proc.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>

void window(uint8_t *img, size_t x, size_t y, size_t width, size_t height, uint8_t *result, uint32_t imageWidth,
            uint32_t imageHeight) {
    // Berechne den Zeiger zum ersten Pixel im Fenster
    uint8_t *windowStart = img + ((imageHeight - y - height) * imageWidth + x * 3);

    // Kopiere den Inhalt des Fensters in das Ergebnis
    for (size_t row = 0; row < height; ++row) {
        memcpy(result + row * ((width * 3) + (width % 4)), windowStart + row * imageWidth, width * 3);
    }
}

void zoom(const uint8_t *img, size_t width, size_t height, size_t scale, uint8_t *result) {

    // Scaled width + Padding (in Bytes)
    size_t zoomedWidth = ((width * scale * 3) + (width * scale * scale % 4));
    // Scaled height in Bytes
    size_t zoomedHeight = height * scale * zoomedWidth - zoomedWidth;
    // Width + padding in Bytes
    size_t paddedWidth = (width * 3) + (width % 4);
    // Ende von Pixel-array
    size_t maxHeight = zoomedHeight + zoomedWidth;
    // Anzahl von zu kopierenden Pixeln
    size_t length = width*height;
    // Anzahl an Layers von Nachbarn
    int radius = (scale - 1) / 2;
    // Array wo die Adressen aller Ursprungspixel gespeichert werden
    size_t *pixelAddr = malloc(length*sizeof(size_t));
    // Wo die zu kopierenden Pixeln gespeichert werden
    size_t temp = 0;
    // Nützliche temporäre index
    size_t index = 0;

    // Kopieren aller Originalpixel
    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++, index++) {
            pixelAddr[index] = zoomedHeight - y * zoomedWidth * scale + x * 3 * scale;
            uint8_t* target_ptr = result + pixelAddr[index];
            uint8_t* source_ptr = img + ((height * paddedWidth - paddedWidth) - y * paddedWidth + x * 3);
            target_ptr[0] = source_ptr[0];
            target_ptr[1] = source_ptr[1];
            target_ptr[2] = source_ptr[2];
        }
    }

    // Nachbarn kopieren
    for (size_t t = 0; t < length; t++) {
        // Pixel das kopiert sein soll
        temp = pixelAddr[t];
        for (int i = -1*radius; i <= radius; i++) {
            // Falls Pixel auf rechte seite steht, soll bissn rechter anfangen
            if (t % width) index = 0;
            else index = radius*3;

            for (int j = -3*radius + index; j <= radius*6 - radius*3; j+=3) {
                if (temp + zoomedWidth * i + j > 0 &&
                    temp + zoomedWidth * i + j < maxHeight)
                        memcpy(result + temp + zoomedWidth*i + j, result + temp, sizeof(uint8_t) * 3);
            }
        }
    }

    if (scale % 2) index = 0;
    else index = 1;

    // Recher Rand
    for (size_t t = width - 1; t < length; t+=width) {
        temp = pixelAddr[t];
        for (int i = -1*radius; i <= radius; i++)
            memcpy(result + temp + (radius+1)*3 + i*zoomedWidth, result + temp, sizeof(uint8_t) * 3 * (radius + index));
    }

    // Falls Lücken
    if (index) {
        // Vertikale Lücken für die erste Reihe
        temp = pixelAddr[0];
        for (int i = -1*radius; i <= 0; i++)
                memcpy(result + temp + (radius + 1)*3 + zoomedWidth*i, result + temp, sizeof(uint8_t) * 3);
        // Andere vertikale Lücken
        for (size_t t = width; t < length; t++) {
            temp = pixelAddr[t];
            for (int i = -1*radius; i <= radius; i++)
                    memcpy(result + temp + (radius + 1)*3 + zoomedWidth*i, result + temp, sizeof(uint8_t) * 3);
        }
        // Horizontale Lücken
        for (size_t t = 0; t < length; t+=width) {
            temp = pixelAddr[t];
            memcpy(result + temp - zoomedWidth*(radius+1), result + temp, sizeof(uint8_t) * 3 * width * scale);
        }
    }

    // Unterer Rand
    for (size_t t = 1; t < radius + 2; t++) {
        memcpy(result + pixelAddr[length-width] - zoomedWidth*(radius+t), 
                result + pixelAddr[length-width], 
                sizeof(uint8_t) * 3 * width * scale);
    }

    free(pixelAddr);

}
