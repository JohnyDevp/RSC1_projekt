#include <stdio.h>
#include <stdint.h>

#include "../src/lib/pixel.h"
#include "../src/lib/graphics.h"

#define TEST_W 16
#define TEST_H 12

static unsigned fb[TEST_W * TEST_H];

static void reset_fb(pixel_t color)
{
    for (int i = 0; i < TEST_W * TEST_H; i++)
    {
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
    return fb[y * TEST_W + x];
}

static int test_1_init_and_getters(void)
{
    graphics_init(fb, TEST_W, TEST_H);
    if (assert_true(graphics_get_width() == TEST_W, "graphics_get_width"))
        return 1;
    if (assert_true(graphics_get_height() == TEST_H, "graphics_get_height"))
        return 1;
    return 0;
}

static int test_2_control_printout(void)
{
    graphics_init(fb, TEST_W, TEST_H);
    control_printout();
    return 0;
}

static int test_3_draw_point(void)
{
    pixel_t red = pixel_rgb(255, 0, 0);
    pixel_t green = pixel_rgb(0, 255, 0);

    graphics_init(fb, TEST_W, TEST_H);
    reset_fb(0);

    graphics_draw_point(3, 4, red);
    graphics_draw_point(-1, 0, green);
    graphics_draw_point(TEST_W, TEST_H, green);

    if (assert_true(pixel_at(3, 4) == (int)red, "draw_point writes in bounds"))
        return 1;
    if (assert_true(pixel_at(0, 0) == 0, "draw_point ignores out of bounds"))
        return 1;
    return 0;
}

static int test_4_draw_line(void)
{
    pixel_t white = pixel_rgb(255, 255, 255);

    graphics_init(fb, TEST_W, TEST_H);
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

static int test_5_draw_rect(void)
{
    pixel_t c = pixel_rgb(10, 20, 30);

    graphics_init(fb, TEST_W, TEST_H);
    reset_fb(0);
    graphics_draw_rect(2, 2, 5, 4, c);

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

static int test_6_draw_rect_filled(void)
{
    pixel_t c = pixel_rgb(40, 50, 60);

    graphics_init(fb, TEST_W, TEST_H);
    reset_fb(0);
    graphics_draw_rect_filled(3, 3, 4, 3, c);

    if (assert_true(pixel_at(3, 3) == (int)c, "filled rect top-left"))
        return 1;
    if (assert_true(pixel_at(6, 5) == (int)c, "filled rect bottom-right"))
        return 1;
    if (assert_true(pixel_at(4, 4) == (int)c, "filled rect interior"))
        return 1;
    return 0;
}

static int test_7_draw_circle(void)
{
    pixel_t c = pixel_rgb(70, 80, 90);

    graphics_init(fb, TEST_W, TEST_H);
    reset_fb(0);
    graphics_draw_circle(6, 6, 2, c);

    if (assert_true(pixel_at(8, 6) == (int)c, "circle right"))
        return 1;
    if (assert_true(pixel_at(4, 6) == (int)c, "circle left"))
        return 1;
    if (assert_true(pixel_at(6, 8) == (int)c, "circle bottom"))
        return 1;
    if (assert_true(pixel_at(6, 4) == (int)c, "circle top"))
        return 1;
    return 0;
}

static int test_8_draw_circle_filled(void)
{
    pixel_t c = pixel_rgb(100, 110, 120);

    graphics_init(fb, TEST_W, TEST_H);
    reset_fb(0);
    graphics_draw_circle_filled(6, 6, 2, c);

    if (assert_true(pixel_at(6, 6) == (int)c, "filled circle center"))
        return 1;
    if (assert_true(pixel_at(7, 7) == (int)c, "filled circle interior"))
        return 1;
    if (assert_true(pixel_at(8, 6) == (int)c, "filled circle boundary"))
        return 1;
    return 0;
}

static int test_9_clear(void)
{
    pixel_t c = pixel_rgb(130, 140, 150);

    graphics_init(fb, TEST_W, TEST_H);
    reset_fb(pixel_rgb(1, 1, 1));
    graphics_clear(c);

    for (int y = 0; y < TEST_H; y++)
    {
        for (int x = 0; x < TEST_W; x++)
        {
            if (pixel_at(x, y) != (int)c)
            {
                printf("FAIL: clear at (%d,%d)\n", x, y);
                return 1;
            }
        }
    }
    return 0;
}

static int test_10_fill(void)
{
    pixel_t c = pixel_rgb(160, 170, 180);

    graphics_init(fb, TEST_W, TEST_H);
    reset_fb(0);
    graphics_fill(c);

    for (int y = 0; y < TEST_H; y++)
    {
        for (int x = 0; x < TEST_W; x++)
        {
            if (pixel_at(x, y) != (int)c)
            {
                printf("FAIL: fill at (%d,%d)\n", x, y);
                return 1;
            }
        }
    }
    return 0;
}

int main(void)
{
#ifndef TEST_CASE
#define TEST_CASE 1
#endif

    int rc = 0;

#if TEST_CASE == 1
    rc = test_1_init_and_getters();
#elif TEST_CASE == 2
    rc = test_2_control_printout();
#elif TEST_CASE == 3
    rc = test_3_draw_point();
#elif TEST_CASE == 4
    rc = test_4_draw_line();
#elif TEST_CASE == 5
    rc = test_5_draw_rect();
#elif TEST_CASE == 6
    rc = test_6_draw_rect_filled();
#elif TEST_CASE == 7
    rc = test_7_draw_circle();
#elif TEST_CASE == 8
    rc = test_8_draw_circle_filled();
#elif TEST_CASE == 9
    rc = test_9_clear();
#elif TEST_CASE == 10
    rc = test_10_fill();
#else
    printf("Unknown TEST_CASE=%d\n", TEST_CASE);
    return 2;
#endif

    if (rc == 0)
    {
        printf("PASS: test_%d\n", TEST_CASE);
    }
    return rc;
}