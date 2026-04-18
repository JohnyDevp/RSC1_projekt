#include "/tmp/ripes_system.h"
#include <stdint.h>
#include <stdio.h>


typedef uint32_t pixel_t;

/* Color constructors */
pixel_t pixel_rgb(uint8_t r, uint8_t g, uint8_t b);
pixel_t pixel_from_hex(uint32_t hex);

/* Color component extractors */
uint8_t pixel_get_r(pixel_t pixel);
uint8_t pixel_get_g(pixel_t pixel);
uint8_t pixel_get_b(pixel_t pixel);


void control_printout();

/* Graphics initialization */
void graphics_init(unsigned *led_base, int width, int height);

/* Basic drawing operations */
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

void graphics_draw_char(int x, int y, char c, pixel_t color);
void graphics_draw_text(int x, int y, const char *str, pixel_t color);


pixel_t pixel_rgb(uint8_t r, uint8_t g, uint8_t b)
{
    return (((pixel_t)r << 16) | ((pixel_t)g << 8) | ((pixel_t)b));
}

pixel_t pixel_from_hex(uint32_t hex)
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


static const uint8_t font3x5[][3] = {
{0x00,0x00,0x00}, // 32 space
{0x00,0x17,0x00}, // 33 !
{0x03,0x00,0x03}, // 34 "
{0x1F,0x0A,0x1F}, // 35 #
{0x12,0x1F,0x09}, // 36 $
{0x19,0x04,0x13}, // 37 %
{0x0A,0x15,0x1A}, // 38 &
{0x00,0x03,0x00}, // 39 '
{0x0E,0x11,0x00}, // 40 (
{0x00,0x11,0x0E}, // 41 )
{0x05,0x02,0x05}, // 42 *
{0x04,0x0E,0x04}, // 43 +
{0x10,0x08,0x00}, // 44 ,
{0x04,0x04,0x04}, // 45 -
{0x00,0x10,0x00}, // 46 .
{0x18,0x04,0x03}, // 47 /
{0x1F,0x11,0x1F}, // 48 0
{0x12,0x1F,0x10}, // 49 1
{0x1D,0x15,0x17}, // 50 2
{0x11,0x15,0x1F}, // 51 3
{0x07,0x04,0x1F}, // 52 4
{0x17,0x15,0x1D}, // 53 5
{0x1F,0x15,0x1D}, // 54 6
{0x01,0x01,0x1F}, // 55 7
{0x1F,0x15,0x1F}, // 56 8
{0x17,0x15,0x1F}, // 57 9
{0x00,0x0A,0x00}, // 58 :
{0x00,0x14,0x00}, // 59 ;
{0x04,0x0A,0x11}, // 60 <
{0x0A,0x0A,0x0A}, // 61 =
{0x11,0x0A,0x04}, // 62 >
{0x01,0x15,0x03}, // 63 ?
{0x0E,0x15,0x16}, // 64 @

{0x1E,0x05,0x1E}, // 65 A
{0x1F,0x15,0x0A}, // 66 B
{0x0E,0x11,0x11}, // 67 C
{0x1F,0x11,0x0E}, // 68 D
{0x1F,0x15,0x15}, // 69 E
{0x1F,0x05,0x05}, // 70 F
{0x0E,0x11,0x1D}, // 71 G
{0x1F,0x04,0x1F}, // 72 H
{0x11,0x1F,0x11}, // 73 I
{0x08,0x10,0x0F}, // 74 J
{0x1F,0x04,0x1B}, // 75 K
{0x1F,0x10,0x10}, // 76 L
{0x1F,0x06,0x1F}, // 77 M
{0x1F,0x0E,0x1F}, // 78 N
{0x0E,0x11,0x0E}, // 79 O
{0x1F,0x05,0x02}, // 80 P
{0x0E,0x19,0x1E}, // 81 Q
{0x1F,0x0D,0x16}, // 82 R
{0x12,0x15,0x09}, // 83 S
{0x01,0x1F,0x01}, // 84 T
{0x0F,0x10,0x0F}, // 85 U
{0x07,0x18,0x07}, // 86 V
{0x1F,0x0C,0x1F}, // 87 W
{0x1B,0x04,0x1B}, // 88 X
{0x03,0x1C,0x03}, // 89 Y
{0x19,0x15,0x13}, // 90 Z

{0x00,0x1F,0x11}, // 91 [
{0x03,0x04,0x18}, // 92 backslash
{0x11,0x1F,0x00}, // 93 ]
{0x02,0x01,0x02}, // 94 ^
{0x10,0x10,0x10}, // 95 _
{0x01,0x02,0x00}, // 96 `
};


void graphics_draw_char(int x, int y, char c, pixel_t color)
{
    if (c < 32 || c > 126)
        return; // unsupported

    const uint8_t *glyph = font3x5[c - 32];

    for (int col = 0; col < 3; col++)
    {
        uint8_t column_data = glyph[col];

        for (int row = 0; row < 5; row++)
        {
            if (column_data & (1 << row))
            {
                graphics_draw_point(x + col, y + row, color);
            }
        }
    }
}

void graphics_draw_text(int x, int y, const char *str, pixel_t color)
{
    int cursor_x = x;

    while (*str)
    {
        graphics_draw_char(cursor_x, y, *str, color);
        cursor_x += 4; // 3 pixels glyph + 1 pixel spacing
        str++;
    }
}


/* LEDs
 * This program draws an animation on an LED matrix peripheral.
 *
 * To run this program, make sure that you have instantiated an "LED Matrix"
 * peripheral in the "I/O" tab.
 */

#define W LED_MATRIX_0_WIDTH
#define H LED_MATRIX_0_HEIGHT
#define LED_MATRIX_0_SIZE (0xdac)
#define LED_MATRIX_0_WIDTH (0x23)
#define LED_MATRIX_0_HEIGHT (0x19)

#define LED_MATRIX_0_BASE (0xf0000000)

char *string = "hello";

void main()
{
    unsigned v = 0;
    graphics_init(LED_MATRIX_0_BASE, W, H);
    control_printout();
    /*graphics_draw_text(1, 1, string, pixel_rgb(255, 255, 255));   */
    graphics_draw_line(0, 0, W - 1, H - 1, pixel_rgb(255, 255, 255));
    graphics_draw_line(0, H - 1, W - 1, 0, pixel_rgb(255, 0, 0));
    graphics_draw_line(0, H / 2, W - 1, H / 2, pixel_rgb(0, 255, 0));
    graphics_draw_line(W / 2, 0, W / 2, H - 1, pixel_rgb(0, 0, 255));

}







