#ifndef PIXEL_H
#define PIXEL_H

#include <stdint.h>

typedef uint32_t pixel_t;

/* Color constructors */
pixel_t pixel_rgb(uint8_t r, uint8_t g, uint8_t b);
pixel_t pixel_from_hex(uint32_t hex);

/* Color component extractors */
uint8_t pixel_get_r(pixel_t pixel);
uint8_t pixel_get_g(pixel_t pixel);
uint8_t pixel_get_b(pixel_t pixel);

#endif /* PIXEL_H */
