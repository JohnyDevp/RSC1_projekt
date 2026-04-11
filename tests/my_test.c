#include <stdio.h>
#include <stdint.h>

#include "../src/lib/pixel.h"
#include "../src/lib/graphics.h"
#include "constant_definitions.h"

static unsigned fb[W * H];

static void reset_fb(pixel_t color)
{
    for (int i = 0; i < W * H; i++)
    {
        // graphics_fill(color);
        fb[i] = color;
    }
}

static int assert_true(int cond, const char *msg)
{
    if (!cond)
    {
        printf("FAIL: %s\n", msg);
        return 1;
    }
    return 0;
}

static int pixel_at(int x, int y)
{
    return fb[y * W + x];
}

int test_1_init_and_getters(void)
{
    graphics_init(LED_MATRIX_0_BASE, W, H);
    printf("Current W H: %u %u\n", W, H);
    // printf("Current W H from getters: %u %u\n", graphics_get_width(), graphics_get_height());
    if (assert_true(graphics_get_width() == W, "graphics_get_width"))
        return 1;
    if (assert_true(graphics_get_height() == H, "graphics_get_height"))
        return 1;
    return 0;
}

int test_2_control_printout(void)
{
    graphics_init(LED_MATRIX_0_BASE, W, H);
    control_printout();
    return 0;
}

int test_3_draw_point(void)
{
    pixel_t red = pixel_rgb(255, 0, 0);
    pixel_t green = pixel_rgb(0, 255, 0);

    graphics_init(LED_MATRIX_0_BASE, W, H);
    reset_fb(0);

    graphics_draw_point(3, 4, red);
    graphics_draw_point(-1, 0, green);
    graphics_draw_point(W, H, green);

    if (assert_true(pixel_at(3, 4) == (int)red, "draw_point writes in bounds"))
        return 1;
    if (assert_true(pixel_at(0, 0) == 0, "draw_point ignores out of bounds"))
        return 1;
    return 0;
}

int test_4_draw_line(void)
{
    pixel_t white = pixel_rgb(255, 255, 255);

    graphics_init(LED_MATRIX_0_BASE, W, H);
    reset_fb(0);
    graphics_draw_line(1, 1, 4, 4, white);

    if (assert_true(pixel_at(1, 1) == (int)white, "line start point"))
        return 1;
    if (assert_true(pixel_at(2, 2) == (int)white, "line midpoint"))
        return 1;
    if (assert_true(pixel_at(3, 3) == (int)white, "line midpoint 2"))
        return 1;
    if (assert_true(pixel_at(4, 4) == (int)white, "line end point"))
        return 1;
    return 0;
}

int test_5_draw_rect(void)
{
    pixel_t c = pixel_rgb(100, 20, 200);

    graphics_init(LED_MATRIX_0_BASE, W, H);
    reset_fb(0);
    graphics_draw_rect(2, 2, 10, 10, c);

    if (assert_true(pixel_at(2, 2) == (int)c, "rect top-left"))
        return 1;
    if (assert_true(pixel_at(6, 2) == (int)c, "rect top-right"))
        return 1;
    if (assert_true(pixel_at(2, 5) == (int)c, "rect bottom-left"))
        return 1;
    if (assert_true(pixel_at(6, 5) == (int)c, "rect bottom-right"))
        return 1;
    if (assert_true(pixel_at(4, 4) == 0, "rect keeps interior empty"))
        return 1;
    return 0;
}

// int test_7_draw_circle(void)
// {
//     pixel_t c = pixel_rgb(70, 80, 90);

//     graphics_init(fb, W, H);
//     reset_fb(0);
//     graphics_draw_circle(6, 6, 2, c);

//     if (assert_true(pixel_at(8, 6) == (int)c, "circle right"))
//         return 1;
//     if (assert_true(pixel_at(4, 6) == (int)c, "circle left"))
//         return 1;
//     if (assert_true(pixel_at(6, 8) == (int)c, "circle bottom"))
//         return 1;
//     if (assert_true(pixel_at(6, 4) == (int)c, "circle top"))
//         return 1;
//     return 0;
// }

// static int test_8_draw_circle_filled(void)
// {
//     pixel_t c = pixel_rgb(100, 110, 120);

//     graphics_init(fb, W, H);
//     reset_fb(0);
//     graphics_draw_circle_filled(6, 6, 2, c);

//     if (assert_true(pixel_at(6, 6) == (int)c, "filled circle center"))
//         return 1;
//     if (assert_true(pixel_at(7, 7) == (int)c, "filled circle interior"))
//         return 1;
//     if (assert_true(pixel_at(8, 6) == (int)c, "filled circle boundary"))
//         return 1;
//     return 0;
// }

// static int test_9_clear(void)
// {
//     pixel_t c = pixel_rgb(130, 140, 150);

//     graphics_init(fb, W, H);
//     reset_fb(pixel_rgb(1, 1, 1));
//     graphics_clear(c);

//     for (int y = 0; y < H; y++)
//     {
//         for (int x = 0; x < W; x++)
//         {
//             if (pixel_at(x, y) != (int)c)
//             {
//                 printf("FAIL: clear at (%d,%d)\n", x, y);
//                 return 1;
//             }
//         }
//     }
//     return 0;
// }

// static int test_10_fill(void)
// {
//     pixel_t c = pixel_rgb(160, 170, 180);

//     graphics_init(fb, W, H);
//     reset_fb(0);
//     graphics_fill(c);

//     for (int y = 0; y < H; y++)
//     {
//         for (int x = 0; x < W; x++)
//         {
//             if (pixel_at(x, y) != (int)c)
//             {
//                 printf("FAIL: fill at (%d,%d)\n", x, y);
//                 return 1;
//             }
//         }
//     }
//     return 0;
// }

void t()
{
    pixel_t c = pixel_rgb(80, 200, 60);

    graphics_init(LED_MATRIX_0_BASE, W, H);
    // reset_fb(0);
    graphics_draw_rect_filled(3, 3, 10, 10, c);
    if (assert_true(pixel_at(3, 3) == (int)c, "filled rect top-left"))
        printf("Checkpoint 1\n");
    // if (assert_true(pixel_at(6, 5) == (int)c, "filled rect bottom-right"))
    //     printf("Checkpoint 2\n");
    // if (assert_true(pixel_at(4, 4) == (int)c, "filled rect interior"))
    //     printf("Checkpoint 3\n");
}

int main(void)
{
    printf("=== LED Matrix Animation Test Started ===\n");
    printf("Test 1: Initialization and Getters\n");
    test_1_init_and_getters();
    t();
    // printf("Test 2: Control Printout\n");
    // test_2_control_printout();
    // printf("Test 3: Draw Point\n");
    // test_3_draw_point();
    // printf("Test 4: Draw Line\n");
    // test_4_draw_line();
    // printf("Test 5: Draw Rect\n");
    // test_5_draw_rect();
    // printf("Test 6: Draw Rect Filled\n");
    // test_6_draw_rect_filled();
    // printf("Test 7: Draw Circle\n");
    // test_7_draw_circle();

    printf("=== LED Matrix Animation Test Finished ===\n");

    return 0;

    // graphics_init(LED_MATRIX_0_BASE, W, H);
    // control_printout();
    // int rc = test_1_init_and_getters();
    // control_printout();
    // printf("PASS: test_%d\n", rc);

    // return rc;
}