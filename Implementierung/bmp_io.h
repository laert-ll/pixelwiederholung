#ifndef BMP_IO_H
#define BMP_IO_H

#include <stdint.h>
#include <stddef.h>

int pixelwiederholung(const char *input, const char *output, size_t x, size_t y, size_t width, size_t height, size_t scale, int version);

#endif
