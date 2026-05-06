#include <stdint.h>

#define W LED_MATRIX_0_WIDTH
#define H LED_MATRIX_0_HEIGHT

// *****************************************************************************
// * LED_MATRIX_0
// *****************************************************************************
#define LED_MATRIX_0_BASE (0xf0000014)
#define LED_MATRIX_0_SIZE (0x8000)
#define LED_MATRIX_0_WIDTH (0x80)
#define LED_MATRIX_0_HEIGHT (0x40)

typedef uint32_t pixel_t;

pixel_t pixel_color_from_rgb(uint8_t r, uint8_t g, uint8_t b)
{
    return (((pixel_t)r << 16) | ((pixel_t)g << 8) | (pixel_t)b);
}

static unsigned *g_led_base = 0;
static int g_width = 0;
static int g_height = 0;

void graphics_init(unsigned *led_base, int width, int height)
{
    g_led_base = led_base;
    g_width = width;
    g_height = height;
}

void graphics_draw_point(int x, int y, pixel_t color)
{
    if (x < 0 || x >= g_width || y < 0 || y >= g_height)
        return;

    unsigned idx = y * g_width + x;
    *(g_led_base + idx) = color;
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

#include "../../pics/image_mountains.h"

int main(void)
{
    graphics_init((unsigned *)LED_MATRIX_0_BASE, W, H);
    // graphics_clear_screen();

    graphics_draw_image_u8(img_mountains, IMG_MOUNTAINS_WIDTH, IMG_MOUNTAINS_HEIGHT, IMG_MOUNTAINS_CHANNELS, 0, 0);

    return 0;
}
