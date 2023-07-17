#ifndef IMG_PROC_H
#define IMG_PROC_H

#include <stdint.h>
#include <stddef.h>

void window(uint8_t *img, size_t x, size_t y, size_t width, size_t height, uint8_t *result, uint32_t imageWidth,
            uint32_t imageHeight);

void zoom(const uint8_t *img, size_t width, size_t height, size_t scale_factor, uint8_t *result, int version);
void runtests();
void test_window_single_pixel();
void test_zoom_scale_factor_3();
void test_zoom_scale_factor_1();
void test_zoom_scale_factor_corner();

#endif
