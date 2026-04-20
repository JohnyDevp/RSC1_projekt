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

/* Compatibility drawing API used by existing code */
void draw_pixel(int x, int y, pixel_t color);
void draw_rect(int x, int y, int width, int height, pixel_t color);
void draw_rect_filled(int x, int y, int width, int height, pixel_t color);
void clear_screen(void);

#endif /* GRAPHICS_H */
