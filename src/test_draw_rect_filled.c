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

    graphics_draw_rect_filled(2, 2, 10, 10, pixel_rgb(80, 200, 60));
    graphics_draw_rect_filled(15, 5, 12, 12, pixel_rgb(200, 60, 150));

    while (1)
    {
    }
}
