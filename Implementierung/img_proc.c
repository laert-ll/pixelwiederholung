#include "bmp_io.h"
#include "img_proc.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>

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
    size_t scaled_width = ((width * scale * 3) + (width * scale * scale % 4));
    // Scaled height in Bytes
    size_t max_scaled_height = height * scale * scaled_width - scaled_width;
    // Width + padding in Bytes
    size_t true_width = (width * 3) + (width % 4);
    // Height in Bytes
    size_t max_height = height * true_width - true_width;
    // Ende von Pixel-array
    size_t max = max_scaled_height + scaled_width;
    // Wo die zu kopierenden Pixeln gespeichert werden
    size_t temp = 0;
    // Anzahl an Layers von Nachbarn
    int radius = (scale - 1) / 2;

    // Kopieren aller Originalpixel
    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            memcpy(result + (max_scaled_height - y * scaled_width * scale + x * 3 * scale),
                    img + (max_height - y * true_width + x * 3),
                    sizeof(uint8_t) * 3);
        }
    }

    // Nachbarn kopieren
    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            // Pixel das kopiert sein soll
            temp = (max_scaled_height - y * scaled_width * scale + x * 3 * scale);

            // Layers von Nachbarn kopieren
            for (int i = 1; i <= radius; i++) {
                // horizontale Schnitt
                for (int j = -3 * i; j <= 3 * i; j += 3) {
                    // vertikale Schnitt
                    for (int k = -1 * radius; k <= radius; k++) {
                        // Out-of-Bounds check
                        if (temp + (scaled_width * k) + j > 0 &&
                            temp + (scaled_width * k) + j < max &&
                            (temp % scaled_width != 0 || j >= 0))
                            memcpy(result + temp + (scaled_width * k) + j, 
                                    result + temp, 
                                    sizeof(uint8_t) * 3);
                    }
                }
            }
        }
    }

    // Falls es Lücken gibt
    if (scale % 2 == 0) {
        for (size_t y = 0; y < height; y++) {
            for (size_t x = 0; x < width; x++) {
                // Ursprungspixel
                temp = (max_scaled_height - y * scaled_width * scale + x * 3 * scale);

                // Lücken auf der rechten Seite
                for (int i = -1 * radius; i <= radius; i++) {
                    if (temp + i*scaled_width + (radius+1)*3 > 0 &&
                        temp + i*scaled_width + (radius+1)*3 < max) {
                            memcpy(result + temp + i*scaled_width + (radius+1)*3,
                                    result + temp, 
                                    sizeof(uint8_t)*3);
                    }
                    // Rechter Rand
                    if (x+1 >= width) {
                        memcpy(result + temp + i*scaled_width + (radius+2)*3,
                                result + temp,
                                sizeof(uint8_t)*3*radius);
                    }
                }
                // Noch was kleines am rechten Rand
                if (x+1 >= width) {
                    memcpy(result + temp + (-1*radius -1)*scaled_width + (radius+2)*3, result + temp, sizeof(uint8_t)*3*radius);
                }

                // Lücken auf der Unterseite
                for (int i = -3 * radius; i <= (radius + 1) * 3; i += 3) {
                    if (temp % scaled_width != 0 || i >= 0) {
                        memcpy(result + temp - scaled_width * (radius + 1) + i, 
                                result + temp, 
                                sizeof(uint8_t) * 3);
                    }
                    // unterer Rand
                    if (y + 1 >= height) {
                        for (int j = 0; j < radius; j++) {
                            // if (temp % scaled_width != 0)
                            memcpy(result + temp - scaled_width * (radius + 2 + j) + i, 
                                    result + temp,
                                    sizeof(uint8_t) * 3);
                        }
                    }
                }

                // Lücke am unteren rechten Rand
                if (x + 1 >= width && y + 1 >= height) {
                    for (int k = 0; k < radius; k++) {
                        memcpy(result + temp - scaled_width * (radius + 2 + k) + (radius + 2) * 3, 
                                result + temp,
                                sizeof(uint8_t) * 3 * radius);
                    }
                }
            }
        }
    }

    // Sonst nur Rand
    else {
        for (size_t y = 0; y < height; y++) {
            for (size_t x = 0; x < width; x++) {
                // Ursprungspixel
                temp = (max_scaled_height - y * scaled_width * scale + x * 3 * scale);

                // Rechter Rand
                for (int i = -1 * radius; i <= radius; i++) {
                    if (x + 1 >= width) {
                        memcpy(result + temp + i * scaled_width + (radius + 1) * 3, 
                                result + temp,
                                sizeof(uint8_t) * 3 * radius);
                    }
                }

                // Unterer Rand
                for (int i = -3 * radius; i <= (radius + 1) * 3; i += 3) {
                    if (y + 1 >= height) {
                        for (int j = 0; j < radius; j++) {
                            memcpy(result + temp - scaled_width * (radius + 1 + j) + i, 
                                    result + temp,
                                    sizeof(uint8_t) * 3);
                        }
                    }
                }

                // Lücke am unteren rechten Rand
                if (x + 1 >= width && y + 1 >= height) {
                    for (int k = 0; k < radius; k++) {
                        memcpy(result + temp - scaled_width * (radius + 1 + k) + (radius + 1) * 3, 
                                result + temp,
                                sizeof(uint8_t) * 3 * radius);
                    }
                }
            }
        }
    }
}
