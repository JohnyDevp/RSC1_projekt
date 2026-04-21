#include "graphics.h"
#include <stdio.h>

/* Global graphics state */

static unsigned *g_led_base = 0;
static int g_width = 0;
static int g_height = 0;

void control_printout()
{
    printf("Current W H LED_BASE: %u %u %u\n", g_width, g_height, g_led_base);
}

void graphics_init(unsigned *led_base, int width, int height)
{
    g_led_base = led_base;
    g_width = width;
    g_height = height;
}

int graphics_get_width(void)
{
    return g_width;
}

int graphics_get_height(void)
{
    return g_height;
}

static int clip_x(int x)
{
    if (x < 0)
        return 0;
    if (x >= g_width)
        return g_width - 1;
    return x;
}

static int clip_y(int y)
{
    if (y < 0)
        return 0;
    if (y >= g_height)
        return g_height - 1;
    return y;
}

void graphics_draw_pixel(int x, int y, pixel_t color)
{
    if (x >= 0 && x < g_width && y >= 0 && y < g_height)
    {
        *((pixel_t *)g_led_base + (y * g_width + x)) = color;
    }
}

void graphics_draw_point(int x, int y, pixel_t color)
{
    if (x < 0 || x >= g_width || y < 0 || y >= g_height)
        return;

    unsigned idx = y * g_width + x;
    *(g_led_base + idx) = color;
}

/* Bresenham's line algorithm */
void graphics_draw_line(int x0, int y0, int x1, int y1, pixel_t color)
{
    int dx = x1 - x0;
    int dy = y1 - y0;

    if (dx < 0)
        dx = -dx;
    if (dy < 0)
        dy = -dy;

    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    int x = x0, y = y0;

    while (1)
    {
        graphics_draw_point(x, y, color);

        if (x == x1 && y == y1)
            break;

        int e2 = 2 * err;

        if (e2 > -dy)
        {
            err -= dy;
            x += sx;
        }

        if (e2 < dx)
        {
            err += dx;
            y += sy;
        }
    }
}

void graphics_draw_rect(int x, int y, int width, int height, pixel_t color)
{
    /* Top and bottom edges */
    for (int i = 0; i < width; i++)
    {
        graphics_draw_point(x + i, y, color);
        graphics_draw_point(x + i, y + height - 1, color);
    }

    /* Left and right edges */
    for (int i = 0; i < height; i++)
    {
        graphics_draw_point(x, y + i, color);
        graphics_draw_point(x + width - 1, y + i, color);
    }
}

void graphics_draw_rect_filled(int x, int y, int width, int height, pixel_t color)
{
    for (int yy = 0; yy < height; yy++)
    {
        for (int xx = 0; xx < width; xx++)
        {
            graphics_draw_point(x + xx, y + yy, color);
        }
    }
}

/* Simple circle drawing using Midpoint Circle Algorithm */
void graphics_draw_circle(int cx, int cy, int radius, pixel_t color)
{
    int x = radius;
    int y = 0;
    int d = 3 - 2 * radius;

    while (x >= y)
    {
        graphics_draw_point(cx + x, cy + y, color);
        graphics_draw_point(cx - x, cy + y, color);
        graphics_draw_point(cx + x, cy - y, color);
        graphics_draw_point(cx - x, cy - y, color);
        graphics_draw_point(cx + y, cy + x, color);
        graphics_draw_point(cx - y, cy + x, color);
        graphics_draw_point(cx + y, cy - x, color);
        graphics_draw_point(cx - y, cy - x, color);

        if (d < 0)
        {
            d = d + 4 * y + 6;
        }
        else
        {
            d = d + 4 * (y - x) + 10;
            x--;
        }
        y++;
    }
}

void graphics_draw_circle_filled(int cx, int cy, int radius, pixel_t color)
{
    for (int y = -radius; y <= radius; y++)
    {
        for (int x = -radius; x <= radius; x++)
        {
            int dx = x;
            int dy = y;
            if (dx * dx + dy * dy <= radius * radius)
            {
                graphics_draw_point(cx + x, cy + y, color);
            }
        }
    }
}

void graphics_clear(pixel_t color)
{
    graphics_draw_rect_filled(0, 0, g_width, g_height, color);
}

void graphics_fill(pixel_t color)
{
    graphics_draw_rect_filled(0, 0, g_width, g_height, color);
}

void graphics_clear_screen()
{
    graphics_draw_rect_filled(0, 0, g_width, g_height, 0x000000);
}

void graphics_draw_buffer(const pixel_t *buffer, int buffer_width, int buffer_height, int dst_x, int dst_y)
{
    if (!buffer || buffer_width <= 0 || buffer_height <= 0)
        return;

    for (int y = 0; y < buffer_height; y++)
    {
        for (int x = 0; x < buffer_width; x++)
        {
            pixel_t color = buffer[y * buffer_width + x];
            graphics_draw_point(dst_x + x, dst_y + y, color);
        }
    }
}

void graphics_draw_image_u8(const uint8_t *image, int image_width, int image_height, int channels, int dst_x, int dst_y)
{
    if (!image || image_width <= 0 || image_height <= 0)
        return;

    if (channels != 1 && channels != 3 && channels != 4)
        return;

    for (int y = 0; y < image_height; y++)
    {
        for (int x = 0; x < image_width; x++)
        {
            int idx = (y * image_width + x) * channels;
            pixel_t color;

            if (channels == 1)
            {
                uint8_t g = image[idx];
                color = pixel_color_from_rgb(g, g, g);
            }
            else
            {
                uint8_t r = image[idx + 0];
                uint8_t g = image[idx + 1];
                uint8_t b = image[idx + 2];
                color = pixel_color_from_rgb(r, g, b);
            }

            graphics_draw_point(dst_x + x, dst_y + y, color);
        }
    }
}
