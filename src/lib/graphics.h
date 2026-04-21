#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "pixel.h"

void control_printout();

/* Graphics initialization */
void graphics_init(unsigned *led_base, int width, int height);

/* Preferred graphics API */
void graphics_draw_point(int x, int y, pixel_t color);
void graphics_draw_line(int x0, int y0, int x1, int y1, pixel_t color);
void graphics_draw_rect(int x, int y, int width, int height, pixel_t color);
void graphics_draw_rect_filled(int x, int y, int width, int height, pixel_t color);
void graphics_draw_circle(int cx, int cy, int radius, pixel_t color);
void graphics_draw_circle_filled(int cx, int cy, int radius, pixel_t color);

/* Utility functions */
void graphics_clear(pixel_t color);
void graphics_fill(pixel_t color);
int graphics_get_width(void);
int graphics_get_height(void);

void graphics_draw_pixel(int x, int y, pixel_t color);
void graphics_clear_screen(void);

/* Draw a pixel buffer (pixel_t values) to LED matrix at destination offset. */
void graphics_draw_buffer(const pixel_t *buffer, int buffer_width, int buffer_height, int dst_x, int dst_y);

/* Convert raw image bytes and draw pixel-by-pixel.
 * channels: 1 = grayscale, 3 = RGB, 4 = RGBA (alpha ignored). */
void graphics_draw_image_u8(const uint8_t *image, int image_width, int image_height, int channels, int dst_x, int dst_y);

#endif /* GRAPHICS_H */
