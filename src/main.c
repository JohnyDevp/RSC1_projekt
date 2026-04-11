#include "/tmp/ripes_system.h"
#include <stdint.h>
#include <stdio.h>

#include "lib/pixel.h"
#include "lib/graphics.h"

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

void main()
{
    unsigned v = 0;

    graphics_init(LED_MATRIX_0_BASE, W, H);
    control_printout();
    while (1)
    {
        for (int y = 0; y < H; y++)
        {
            for (int x = 0; x < W; x++)
            {
                char r = v + (0xFF * x) / W;
                char g = v + (0xFF * y) / H;
                char b = v + (0xFF * (x + y)) / (W + H);
                pixel_t color = pixel_rgb(r, g, b);
                graphics_draw_point(x, y, color);
            }
            v++;
        }
    }
}