#ifndef BMP_IO_H
#define BMP_IO_H

#include <stdint.h>
#include <stddef.h>

void read_image(const char *input, const char *output);

void write_image(uint8_t *header, const uint8_t *img, uint32_t width, uint32_t height, const char *output);

void window_test(const char *input, const char *output, size_t x, size_t y, size_t width, size_t height);

#endif
