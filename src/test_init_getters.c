#include "/tmp/ripes_system.h"
#include <stdint.h>
#include <stdio.h>

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
    printf("W=%d H=%d\n", graphics_get_width(), graphics_get_height());

    while (1)
    {
    }
}
