// #include "/tmp/ripes_system.h"
#include <stdint.h>
#include <stdio.h>

#include "lib/graphics.h"
#include "lib/pixel.h"

#define W LED_MATRIX_0_WIDTH
#define H LED_MATRIX_0_HEIGHT
#define W 128
#define H 64

// *****************************************************************************
// * SWITCHES_0
// *****************************************************************************
#define SWITCHES_0_BASE (0xf0000000)
#define SWITCHES_0_SIZE (0x4)
#define SWITCHES_0_N (0x8)

// *****************************************************************************
// * D_PAD_0
// *****************************************************************************
#define D_PAD_0_BASE (0xf0000004)
#define D_PAD_0_SIZE (0x10)
#define D_PAD_0_UP_OFFSET (0x0)
#define D_PAD_0_UP (0xf0000004)
#define D_PAD_0_DOWN_OFFSET (0x4)
#define D_PAD_0_DOWN (0xf0000008)
#define D_PAD_0_LEFT_OFFSET (0x8)
#define D_PAD_0_LEFT (0xf000000c)
#define D_PAD_0_RIGHT_OFFSET (0xc)
#define D_PAD_0_RIGHT (0xf0000010)

// *****************************************************************************
// * LED_MATRIX_0
// *****************************************************************************
#define LED_MATRIX_0_BASE (0xf0000014)
#define LED_MATRIX_0_SIZE (0x8000)
#define LED_MATRIX_0_WIDTH (0x80)
#define LED_MATRIX_0_HEIGHT (0x40)

int main(void)
{
    graphics_init((unsigned *)LED_MATRIX_0_BASE, W, H);
    graphics_clear_screen();
    graphics_draw_circle_filled(W / 2, H / 2, 20, pixel_color_from_rgb(255, 0, 0));
    graphics_draw_circle(W / 2 + 20, H / 2 + 20, 20, pixel_color_from_rgb(0, 255, 0));
    graphics_draw_line(0, 0, W - 1, H - 1, pixel_color_from_rgb(0, 0, 255));
    graphics_draw_rect(10, 10, 50, 30, pixel_color_from_rgb(255, 255, 0));
    graphics_draw_rect_filled(20, 20, 50, 30, pixel_color_from_rgb(0, 255, 255));
    return 0;
}
