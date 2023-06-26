#include "img_proc.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>


void window(uint8_t *img, size_t x, size_t y, size_t width, size_t height, uint8_t *result, uint32_t imageWidth,
            uint32_t imageHeight) {
    // Überprüfe die Grenzen des Fensters
    if (x + width > imageWidth || y + height > imageHeight) {
        printf("Error: The window is outside the image borders");
        return;
    }

    // Berechne den Zeiger zum ersten Pixel im Fenster
    uint8_t *windowStart = img + (y * imageWidth + x) * 3;

    // Kopiere den Inhalt des Fensters in das Ergebnis
    for (size_t row = 0; row < height; ++row) {
        memcpy(result + row * width * 3, windowStart + row * imageWidth * 3, width * 3);
    }
}

void zoom(const uint8_t *img, size_t width, size_t height, size_t scale_factor, uint8_t *result) {
    
    //Kopiere einzelne Felder von window mit passenden Abständen nach result
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            result[scale_factor*scale_factor*width*i] = img[(i*width) + j]; //i*width = current row, j = current column
        }
    }

    //MUSS GETESTET WERDEN



}