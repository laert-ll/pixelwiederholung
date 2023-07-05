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

void zoom(const uint8_t *img, size_t width, size_t height, size_t scale, uint8_t *result) {

    size_t scaled_width = ((width*scale * 3) + (width*scale*scale % 4));
    size_t max_scaled_height = height*scale*scaled_width - scaled_width;
    size_t true_width = (width*3) + (width % 4);
    size_t max_height = height*true_width - true_width;

    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            memcpy(result + (max_scaled_height - y*scaled_width*scale + x*3*scale), 
                    img + (max_height - y*true_width + x*3), 
                    sizeof(uint8_t)*3);
        }
    }

    size_t temp = 0;
    int radius = (scale - 1) / 2;

    size_t max = max_scaled_height + scaled_width;
    for (size_t y = 0; y < height; y++) {

        for (size_t x = 0; x < width; x++) {
            temp = (max_scaled_height - y*scaled_width*scale + x*3*scale);

            for (int i = 1; i <= radius; i++) {

                for (int j = -3 * i ; j <= 3*i; j+= 3) {

                    for (int k = -1 * radius; k <= radius; k++) {

                        if (temp + (scaled_width*k) + j > 0 && temp + (scaled_width*k) + j < max && (temp % scaled_width != 0 || j >=0))
                            memcpy(result + temp + (scaled_width*k) + j, result + temp, sizeof(uint8_t)*3);

                    }

                }

            }

        }

    }

    if (scale % 2 == 0) {

        for (size_t y = 0; y < height; y++) {
            for (size_t x = 0; x < width; x++) {
                temp = (max_scaled_height - y*scaled_width*scale + x*3*scale);

                // right side
                for (int i = -1 * radius; i <= radius; i++) {

                    if (temp + i*scaled_width + (radius+1)*3 > 0 && temp + i*scaled_width + (radius+1)*3 < max) {
                            memcpy(result + temp + i*scaled_width + (radius+1)*3, result + temp, sizeof(uint8_t)*3);
                    }
                    if (x+1 >= width) {
                        memcpy(result + temp + i*scaled_width + (radius+2)*3, result + temp, sizeof(uint8_t)*3*radius);
                    }
                }
                if (x+1 >= width) {
                    memcpy(result + temp + (-1*radius -1)*scaled_width + (radius+2)*3, result + temp, sizeof(uint8_t)*3*radius);
                }

                // bottom
                for (int i = -3 * radius; i <= (radius+1)*3; i+=3) {

                    
                    if (y+1 >= height) {
                        for(int j = 0; j < radius; j++) {
                            // if (temp % scaled_width != 0)
                               memcpy(result + temp - scaled_width*(radius+2+j) + i, result + temp, sizeof(uint8_t)*3);
                        }
                    }

                    if (temp % scaled_width != 0 || i >=0) {
                        memcpy(result + temp - scaled_width*(radius+1) + i, result + temp, sizeof(uint8_t)*3);
                    }
                }

                // bottom-right edge
                if (x+1 >= width && y+1 >= height) {
                    for (int k = 0; k < radius; k++) {
                        memcpy(result + temp - scaled_width*(radius+2+k) + (radius+2)*3,  result + temp, sizeof(uint8_t)*3*radius);
                    }
                }


            }
        }
    }

    else {
        for (size_t y = 0; y < height; y++) {
            for (size_t x = 0; x < width; x++) {
                temp = (max_scaled_height - y*scaled_width*scale + x*3*scale);

                
                // right side
                for (int i = -1 * radius; i <= radius; i++) {
                    if (x+1 >= width) {
                        memcpy(result + temp + i*scaled_width + (radius+1)*3, result + temp, sizeof(uint8_t)*3*radius);
                    }
                }

                // bottom
                for (int i = -3 * radius; i <= (radius+1)*3; i+=3) {
                    
                    if (y+1 >= height) {
                        for(int j = 0; j < radius; j++) {
                                memcpy(result + temp - scaled_width*(radius+1+j) + i, result + temp, sizeof(uint8_t)*3);
                        }
                    }
                }
                
                // bottom-right edge
                if (x+1 >= width && y+1 >= height) {
                    for (int k = 0; k < radius; k++) {
                        memcpy(result + temp - scaled_width*(radius+1+k) + (radius+1)*3,  result + temp, sizeof(uint8_t)*3*radius);
                    }
                }
            }
        }
    }

}
