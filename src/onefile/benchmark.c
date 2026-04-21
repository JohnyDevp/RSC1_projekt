#include <stdint.h>
#include <stdio.h>

#define W LED_MATRIX_0_WIDTH
#define H LED_MATRIX_0_HEIGHT

// *****************************************************************************
// * LED_MATRIX_0
// *****************************************************************************
#define LED_MATRIX_0_BASE (0xf0000000)
#define LED_MATRIX_0_SIZE (0x8000)
#define LED_MATRIX_0_WIDTH (0x80)
#define LED_MATRIX_0_HEIGHT (0x40)

// *****************************************************************************
// * SWITCHES_0
// *****************************************************************************
#define SWITCHES_0_BASE (0xf0008000)
#define SWITCHES_0_SIZE (0x4)
#define SWITCHES_0_N (0x8)

// *****************************************************************************
// * D_PAD_0
// *****************************************************************************

#define D_PAD_0_BASE (0xf0008004)
#define D_PAD_0_SIZE (0x10)
#define D_PAD_0_UP_OFFSET (0x0)
#define D_PAD_0_UP (0xf0008004)
#define D_PAD_0_DOWN_OFFSET (0x4)
#define D_PAD_0_DOWN (0xf0008008)
#define D_PAD_0_LEFT_OFFSET (0x8)
#define D_PAD_0_LEFT (0xf000800c)
#define D_PAD_0_RIGHT_OFFSET (0xc)
#define D_PAD_0_RIGHT (0xf0008010)

// ******************************************************************************
// inlucded functions

// ******************************************************************************
typedef uint32_t pixel_t;

pixel_t pixel_color_from_rgb(uint8_t r, uint8_t g, uint8_t b)
{
    return (((pixel_t)r << 16) | ((pixel_t)g << 8) | (pixel_t)b);
}

pixel_t pixel_color_from_hex(uint32_t hex)
{
    return hex;
}

// ******************************************************************************
static unsigned *g_led_base = 0;
static int g_width = 0;
static int g_height = 0;

void graphics_init(unsigned *led_base, int width, int height)
{
    g_led_base = led_base;
    g_width = width;
    g_height = height;
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

void graphics_clear_screen()
{
    graphics_draw_rect_filled(0, 0, g_width, g_height, 0x000000);
}

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

int main(void)
{
    graphics_init((unsigned *)LED_MATRIX_0_BASE, W, H);

    // draw random shapes
    graphics_clear_screen();

    graphics_draw_circle_filled(W / 2, H / 2, 20, pixel_color_from_rgb(255, 0, 0));
    graphics_draw_circle(W / 2 + 10, H / 2 + 10, 10, pixel_color_from_rgb(0, 255, 0));
    graphics_draw_line(0, 0, W - 1, H - 1, pixel_color_from_rgb(0, 0, 255));
    graphics_draw_rect(10, 10, 50, 30, pixel_color_from_rgb(255, 255, 0));
    graphics_draw_rect_filled(20, 20, 50, 30, pixel_color_from_rgb(0, 255, 255));

    // draw me a smiley face with background
    graphics_clear_screen();

    graphics_draw_circle(W / 2, H / 2, 30, pixel_color_from_rgb(255, 255, 0));
    // draw eyes
    graphics_draw_circle_filled(W / 2 - 10, H / 2 - 10, 5, pixel_color_from_rgb(250, 100, 0));
    graphics_draw_circle_filled(W / 2 + 10, H / 2 - 10, 5, pixel_color_from_rgb(250, 100, 0));
    // draw nose
    graphics_draw_line(W / 2, H / 2 - 5, W / 2, H / 2 + 5, pixel_color_from_rgb(250, 100, 0));
    // draw smile by points
    for (int i = -10; i <= 10; i++)
    {
        int x = W / 2 + i;
        int y = H / 2 + 20 - (i * i) / 20; // parabola for smile
        graphics_draw_point(x, y, pixel_color_from_rgb(250, 100, 0));
    }

    return 0;
}
