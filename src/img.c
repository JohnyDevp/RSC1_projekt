#include <stdint.h>
#include <stdio.h>
#include "lib/graphics.h"
#include "lib/pixel.h"

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

// the image
#include "../pics/image_palm.h"
#include "../pics/image_tree.h"

int main()
{
    graphics_init((unsigned *)LED_MATRIX_0_BASE, W, H);

    graphics_clear_screen();

    // random image
    const int iw = 4;
    const int ih = 4;
    const uint8_t img_rgb2[4 * 4 * 3] = {
        255,0,0,   0,255,0,   0,0,255,   255,255,0,
        255,0,255, 0,255,255, 255,255,255, 32,32,32,
        200,50,50, 50,200,50, 50,50,200, 180,120,0,
        0,0,0,     80,80,80,  160,160,160, 240,240,240
    };

    graphics_draw_image_u8(img_rgb2, iw, ih, 3, 40, 12);

    // palm image
    graphics_draw_image_u8(img_palm, IMG_PALM_WIDTH, IMG_PALM_HEIGHT, IMG_PALM_CHANNELS, 60, 12);

    // tree image
    graphics_draw_image_u8(img_tree, IMG_TREE_WIDTH, IMG_TREE_HEIGHT, IMG_TREE_CHANNELS, 2, 30);

    // random image
    const uint8_t img_gray[4 * 4] = {
        0, 64, 128, 255,
        255, 128, 64, 0,
        32, 96, 160, 224,
        224, 160, 96, 32
    };

    graphics_draw_image_u8(img_gray, 4, 4, 1, 50, 20);

    return 0;
}