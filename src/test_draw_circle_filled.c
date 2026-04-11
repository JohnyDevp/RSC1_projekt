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
    graphics_clear(pixel_rgb(0, 0, 0));

    graphics_draw_circle_filled(10, 8, 5, pixel_rgb(0, 120, 255));
    graphics_draw_circle_filled(24, 12, 6, pixel_rgb(255, 120, 0));

    while (1)
    {
    }
}
