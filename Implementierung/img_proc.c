#include "bmp_io.h"
#include "img_proc.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>

void window(uint8_t *img, size_t x, size_t y, size_t width, size_t height, uint8_t *result, uint32_t imageWidth,
            uint32_t imageHeight) {

    // Berechne den Zeiger zum ersten Pixel im Fenster
    uint8_t *windowStart = img + (y * imageWidth + x * 3);

    // Kopiere den Inhalt des Fensters in das Ergebnis
    for (size_t row = 0; row < height; ++row) {
        memcpy(result + row * ((width * 3) + (width % 4)),
               windowStart + row * imageWidth, width * 3);
    }
}

void zoom(const uint8_t *img, size_t width, size_t height, size_t scale_factor, uint8_t *result) {
    // Idee: x/y-Koordinate * scale_factor
    size_t new_width = width * scale_factor;


    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            uint8_t *currentPixel = img + (y * width + x * 3);
            printf("Pixel:  %u\n", currentPixel);
            memcpy(result + y * ((width * 3) + (width % 4)) * scale_factor, currentPixel + row * width, width * 3);
        }
    }
}
