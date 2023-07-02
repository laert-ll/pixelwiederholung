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

// vorläufig, muss getestet werden
void zoom(const uint8_t *img, size_t width, size_t height, size_t scale_factor, uint8_t *result) {
    // TODO: machen
}
