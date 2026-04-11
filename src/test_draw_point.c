#include "/tmp/ripes_system.h"
#include <stdint.h>

#include "lib/pixel.h"
#include "lib/graphics.h"

#define W LED_MATRIX_0_WIDTH
#define H LED_MATRIX_0_HEIGHT
#define LED_MATRIX_0_WIDTH (0x23)
#define LED_MATRIX_0_HEIGHT (0x19)
#define LED_MATRIX_0_BASE (0xf0000000)

void main()
{
    graphics_init(LED_MATRIX_0_BASE, W, H);
    // graphics_clear(pixel_rgb(0, 0, 0));

    graphics_draw_point(W / 2, H / 2, pixel_rgb(255, 255, 255));
    graphics_draw_point(1, 1, pixel_rgb(255, 0, 0));
    graphics_draw_point(W - 2, 1, pixel_rgb(0, 255, 0));
    graphics_draw_point(1, H - 2, pixel_rgb(0, 0, 255));
    graphics_draw_point(W - 2, H - 2, pixel_rgb(255, 255, 0));

    while (1)
    {
    }
}
