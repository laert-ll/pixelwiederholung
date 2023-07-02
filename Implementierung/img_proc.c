#include "bmp_io.h"
#include "img_proc.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>

const char *getClearName(uint8_t r, uint8_t g, uint8_t b) {
    if (r > 200 && g < 50 && b < 50) {
        return "Red";
    } else if (r < 50 && g > 200 && b < 50) {
        return "Green";
    } else if (r > 200 && g > 200 && b > 200) {
        return "White";
    } else if (r < 50 && g < 50 && b > 200) {
        return "Blue";
    } else {
        return "Unknown";
    }
}


void window(uint8_t *img, size_t x, size_t y, size_t width, size_t height, uint8_t *result, uint32_t imageWidth,
            uint32_t imageHeight) {
    // Berechne den Zeiger zum ersten Pixel im Fenster
    uint8_t *windowStart = img + ((imageHeight - y - height) * imageWidth + x * 3);

    // Kopiere den Inhalt des Fensters in das Ergebnis
    for (size_t row = 0; row < height; ++row) {
        memcpy(result + row * ((width * 3) + (width % 4)), windowStart + row * imageWidth, width * 3);
    }
}

void zoom(const uint8_t *img, size_t width, size_t height, size_t scale_factor, uint8_t *result) {
    // Idee: x/y-Koordinate * scale_factor
    size_t zoomedWidth = width * scale_factor;
    size_t zoomedHeight = height * scale_factor;

    for (size_t y = 0; y < height; ++y) {
        for (size_t x = 0; x < width; ++x) {
            int offset = (y * width + x) * 3;
            uint8_t *pixel = img + offset;
            printf("%s\n", &pixel);
            printf("Here: %s\n", getClearName(pixel[0], pixel[1], pixel[2]));

            for (size_t i = 0; i < scale_factor; ++i) {
                for (size_t j = 0; j < scale_factor; ++j) {
                    size_t zoomedX = x * scale_factor + j;
                    size_t zoomedY = y * scale_factor + i;

                    uint8_t *zoomedPixel = result + (zoomedY * zoomedWidth + zoomedX) * 3;
                    printf("Zoomed Pixel at (%zu,%zu): R=%u, G=%u, B=%u\n", zoomedX, zoomedY, zoomedPixel[0],
                           zoomedPixel[1], zoomedPixel[2]);
                    memcpy(zoomedPixel, pixel, 3);
                }
            }
        }
    }
}

