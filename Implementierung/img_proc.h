#ifndef IMG_PROC_H
#define IMG_PROC_H

#include <stdint.h>
#include <stddef.h>

void window(uint8_t* img, size_t x, size_t y, size_t width, size_t height, uint8_t* result);
void zoom(const uint8_t* img, size_t width, size_t height, size_t scale_factor, uint8_t* result);

#endif
