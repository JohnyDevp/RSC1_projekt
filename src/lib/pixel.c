#include "pixel.h"

pixel_t pixel_color_from_rgb(uint8_t r, uint8_t g, uint8_t b)
{
    return (((pixel_t)r << 16) | ((pixel_t)g << 8) | (pixel_t)b);
}

pixel_t pixel_color_from_hex(uint32_t hex)
{
    return hex;
}

uint8_t pixel_get_r(pixel_t pixel)
{
    return (pixel >> 16) & 0xFF;
}

uint8_t pixel_get_g(pixel_t pixel)
{
    return (pixel >> 8) & 0xFF;
}

uint8_t pixel_get_b(pixel_t pixel)
{
    return pixel & 0xFF;
}
