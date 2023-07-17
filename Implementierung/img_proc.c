#include "bmp_io.h"
#include "img_proc.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

void window(uint8_t *img, size_t x, size_t y, size_t width, size_t height, uint8_t *result, uint32_t imageWidth,
            uint32_t imageHeight) {
    // Berechne den Zeiger zum ersten Pixel im Fenster
    uint8_t *windowStart = img + ((imageHeight - y - height) * imageWidth + x * 3);

    // Kopiere den Inhalt des Fensters in das Ergebnis
    for (size_t row = 0; row < height; ++row) {
        memcpy(result + row * ((width * 3) + (width % 4)), windowStart + row * imageWidth, width * 3);
    }
}

void zoom(const uint8_t *img, size_t width, size_t height, size_t scale_factor, uint8_t *result, int version) {

    // Skalierte width + Padding in Bytes
    size_t zoomedWidth = (width * scale_factor * 3) + (width * scale_factor % 4);
    // Skalierte height in Bytes
    size_t zoomedHeight = height * scale_factor * zoomedWidth - zoomedWidth;
    // Width + Padding in Bytes (Originalbild)
    size_t paddedWidth = (width * 3) + (width % 4);
    // Ende von (skalierte) Pixel-array
    size_t maxHeight = zoomedHeight + zoomedWidth;
    // Anzahl von zu kopierenden Pixeln
    size_t length = width * height;
    // Anzahl an Layers von Nachbarn
    int radius = (scale_factor - 1) / 2;
    // Array wo die Adressen aller Originalpixel gespeichert werden
    size_t *pixelAddresses = malloc(length * sizeof(size_t));
    // Wo die zu kopierenden Pixeln gespeichert werden
    size_t temp = 0;
    // Nützliche temporäre index
    size_t index = 0;

    // Schritt 1: Kopieren aller Originalpixel

    // Alle Reihen des Originalbildes durch
    for (size_t y = 0; y < height; y++) {
        // Jedes Pixel in der Reihe durch
        for (size_t x = 0; x < width; x++, index++) {
            // Wir verwenden einen Offset von y-mal zoomedWidth, um zur nächsten Reihe zu gehen
            // und noch ein Offset für innerhalb der Reihe
            pixelAddresses[index] = zoomedHeight - y * zoomedWidth * scale_factor + x * 3 * scale_factor;
            memcpy(result + pixelAddresses[index],
                   img + (height - 1 - y) * paddedWidth + x * 3,
                   sizeof(uint8_t) * 3);
        }
    }

    // Schritt 2: Nachbarn kopieren
    for (size_t t = 0; t < length; t++) {
        // Pixel das kopiert sein soll
        temp = pixelAddresses[t];

        // Falls Pixel auf rechte seite steht, das Kopieren soll rechter anfangen
        if (t % width) index = 0;
        else index = radius*3;

        // Alle Reihen durch
        for (int i = -1 * radius; i <= radius; i++) {
            // Jedes Pixel in der Reihe durch
            // Wir gehen für jeden Pixel zu dritt weiter
            for (int j = -3 * radius + index; j < radius * 6; j += 3) {
                // Überprüfen, ob out of bounds
                if (temp + zoomedWidth * i + j > 0 &&
                    temp + zoomedWidth * i + j < maxHeight)
                        memcpy(result + temp + zoomedWidth * i + j, 
                               result + temp, 
                               sizeof(uint8_t) * 3);
            }
        }
    }

    // Schritt 3: Lücken und Ränder ausfüllen

    // Falls Skalierungsfaktor ungerade, Rand gleich breit wie Radius,
    // sont Randbreite = radius + 1
    if (scale_factor % 2) index = 0;
    else index = 1;

    // Recher Rand
    for (size_t t = width - 1; t < length; t += width) {
        temp = pixelAddresses[t];
        for (int i = -1 * radius; i <= radius; i++)  {
            if (!(t < width && i > 0))
                // Ganze Zeile von rechts kopieren
                memcpy(result + temp + (radius + 1) * 3 + i * zoomedWidth,
                       result + temp,
                       sizeof(uint8_t) * 3 * (radius + index));
        }
    }

    // Falls Lücken
    if (index) {
        // Vertikale Lücken
        for (size_t t = 0; t < length; t++) {
            temp = pixelAddresses[t];
            for (int i = -1 * radius; i <= radius; i++) {
                if (!(t < width && i > 0))
                    memcpy(result + temp + (radius + 1) * 3 + zoomedWidth * i,
                           result + temp,
                           sizeof(uint8_t) * 3);
            }
        }

        // Horizontale Lücken
        for (size_t t = 0; t < length; t += width) {
            temp = pixelAddresses[t];
            memcpy(result + temp - zoomedWidth * (radius + 1),
                   result + temp,
                   sizeof(uint8_t) * 3 * width * scale_factor);
        }
    }

    // Unterer Rand
    for (size_t t = 1; t < radius + index + 1; t++) {
        // Die unterste Zeile (radius + index + 1)-mal darunter kopieren
        memcpy(result + pixelAddresses[length - width] - zoomedWidth * (radius + t),
               result + pixelAddresses[length - width],
               sizeof(uint8_t) * 3 * width * scale_factor);
    }

    free(pixelAddresses);

}

void test_zoom_scale_factor_1() {
    // Arrange
    uint8_t img[24] = {255, 0, 0,  // Rot
                       0, 0, 255,  // Blau
                       0, 255, 0,  // Grün
                       255, 255, 255}; // Weiß
    size_t width = 2;
    size_t height = 2;
    size_t scale_factor = 1;
    uint8_t result[216];

    // Act
    zoom(img, width, height, scale_factor, result, 0);

    // Assert
    assert(result[0] == 255);
    assert(result[1] == 0);
    assert(result[2] == 0);
    assert(result[3] == 0);
    assert(result[4] == 0);
    assert(result[5] == 255);
    assert(result[6] == 0);
    assert(result[7] == 0);
    assert(result[8] == 0);
    assert(result[9] == 255);
    assert(result[10] == 255);
    assert(result[11] == 255);

    printf("test_zoom_scale_factor_1 erfolgreich durchgeführt.\n");
}

void test_zoom_scale_factor_3() {
    // Arrange
    uint8_t img[24] = {255, 0, 0,  // Rot
                       0, 0, 255,  // Blau
                       0, 255, 0,  // Grün
                       255, 255, 255}; // Weiß
    size_t width = 2;
    size_t height = 2;
    size_t scale_factor = 3;
    uint8_t result[216]; // Skaliertes Bild mit 6x6 Pixeln (24 Bytes pro Pixel)

    // Act
    zoom(img, width, height, scale_factor, result, 0);

    // Assert
    assert(result[0] == 255);    // Rot
    assert(result[1] == 0);
    assert(result[2] == 0);
    assert(result[3] == 255);    // Rot
    assert(result[4] == 0);
    assert(result[5] == 0);
    assert(result[6] == 0);      // Blau
    assert(result[7] == 0);
    assert(result[8] == 255);
    assert(result[9] == 0);      // Blau
    assert(result[10] == 0);
    assert(result[11] == 255);
    assert(result[12] == 0);     // Blau
    assert(result[13] == 0);
    assert(result[14] == 255);
    assert(result[15] == 0);     // Blau
    assert(result[16] == 0);
    assert(result[17] == 255);
    //
    assert(result[20] == 255);   // Rot
    assert(result[21] == 0);
    assert(result[22] == 0);
    assert(result[23] == 255);   // Rot
    assert(result[24] == 0);
    assert(result[25] == 0);
    assert(result[26] == 0);     // Blau
    assert(result[27] == 0);
    assert(result[28] == 255);
    assert(result[29] == 0);     // Blau
    assert(result[30] == 0);
    assert(result[31] == 255);
    assert(result[32] == 0);     // Blau
    assert(result[33] == 0);
    assert(result[34] == 255);
    assert(result[35] == 0);     // Blau
    assert(result[36] == 0);
    assert(result[37] == 255);
    // ...
    //
    printf("test_zoom_scale_factor_3 erfolgreich durchgeführt.\n");
}

void test_window_single_pixel() {
    uint8_t img[3] = {255, 0, 0}; // Ein roter Pixel
    uint8_t result[3];
    window(img, 0, 0, 1, 1, result, 1, 1);
    // Erwartetes Ergebnis: Der rote Pixel
    assert(result[0] == 255 && result[1] == 0 && result[2] == 0);
    printf("test_window_single_pixel erfolgreich durchgeführt.\n");
}

void test_zoom_scale_factor_corner() {
    // Arrange
    uint8_t img[24] = {255, 0, 0,  // Rot
                       0, 0, 255,  // Blau
                       0, 255, 0,  // Grün
                       255, 255, 255}; // Weiß
    size_t width = 2;
    size_t height = 2;
    size_t scale_factor = 3;
    uint8_t result[216]; // Skaliertes Bild mit 6x6 Pixeln (24 Bytes pro Pixel)

    // Act
    zoom(img, width, height, scale_factor, result, 0);

    // Assert
    // Top-Left Corner:
    assert(result[0] == 255);    // Rot
    assert(result[1] == 0);
    assert(result[2] == 0);
    // Top-Right Corner:
    assert(result[15] == 0);     // Blau
    assert(result[16] == 0);
    assert(result[17] == 255);
    
    printf("test_zoom_scale_factor_corner erfolgreich durchgeführt.\n");
}

void runtests() {
    test_zoom_scale_factor_3();
    test_window_single_pixel();
    test_zoom_scale_factor_1();
    test_zoom_scale_factor_corner();
}

