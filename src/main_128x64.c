#include "ripes_system.h"
#include <stdint.h>
#include <stdio.h>

#define W LED_MATRIX_0_WIDTH
#define H LED_MATRIX_0_HEIGHT
#define W 128
#define H 64

// LED matrix adresy
#define LED_MATRIX_0_BASE (0xf0000014)
#define LED_MATRIX_0_SIZE (0x8000)
#define LED_MATRIX_0_WIDTH (0x80)
#define LED_MATRIX_0_HEIGHT (0x40)

// Switches adresy
#define SWITCHES_0_BASE (0xf0000000)
#define SWITCHES_0_SIZE (0x4)
#define SWITCHES_0_N (0x8)

// D-Pad adresy
#define D_PAD_0_BASE	(0xf0000004)
#define D_PAD_0_SIZE	(0x10)
#define D_PAD_0_UP_OFFSET	(0x0)
#define D_PAD_0_UP	(0xf0000004)
#define D_PAD_0_DOWN_OFFSET	(0x4)
#define D_PAD_0_DOWN	(0xf0000008)
#define D_PAD_0_LEFT_OFFSET	(0x8)
#define D_PAD_0_LEFT	(0xf000000c)
#define D_PAD_0_RIGHT_OFFSET	(0xc)
#define D_PAD_0_RIGHT	(0xf0000010)

volatile uint32_t* switches = (uint32_t*)SWITCHES_0_BASE;


#define BRICK_ROWS 3
#define BRICK_COLS 7
#define BRICK_W 5
#define BRICK_H 2
uint8_t bricks[BRICK_ROWS][BRICK_COLS];
typedef uint32_t pixel_t;
static unsigned *g_led_base = 0;
static int g_width = 0;
static int g_height = 0;

void control_printout()
{
    printf("Current W H LED_BASE: %u %u %u\n", g_width, g_height, g_led_base);
}

void graphics_init(unsigned *led_base, int width, int height)
{
    g_led_base = led_base;
    g_width = width;
    g_height = height;
}

int graphics_get_width(void)
{
    return g_width;
}

int graphics_get_height(void)
{
    return g_height;
}

static int clip_x(int x)
{
    if (x < 0)
        return 0;
    if (x >= g_width)
        return g_width - 1;
    return x;
}

static int clip_y(int y)
{
    if (y < 0)
        return 0;
    if (y >= g_height)
        return g_height - 1;
    return y;
}

void graphics_draw_point(int x, int y, pixel_t color)
{
    if (x < 0 || x >= g_width || y < 0 || y >= g_height)
        return;

    unsigned idx = y * g_width + x;
    *(g_led_base + idx) = color;
}

/* Bresenham's line algorithm */
void graphics_draw_line(int x0, int y0, int x1, int y1, pixel_t color)
{
    int dx = x1 - x0;
    int dy = y1 - y0;

    if (dx < 0)
        dx = -dx;
    if (dy < 0)
        dy = -dy;

    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    int x = x0, y = y0;

    while (1)
    {
        graphics_draw_point(x, y, color);

        if (x == x1 && y == y1)
            break;

        int e2 = 2 * err;

        if (e2 > -dy)
        {
            err -= dy;
            x += sx;
        }

        if (e2 < dx)
        {
            err += dx;
            y += sy;
        }
    }
}

void graphics_draw_rect(int x, int y, int width, int height, pixel_t color)
{
    /* Top and bottom edges */
    for (int i = 0; i < width; i++)
    {
        graphics_draw_point(x + i, y, color);
        graphics_draw_point(x + i, y + height - 1, color);
    }

    /* Left and right edges */
    for (int i = 0; i < height; i++)
    {
        graphics_draw_point(x, y + i, color);
        graphics_draw_point(x + width - 1, y + i, color);
    }
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
void draw_rect(int x, int y, int width, int height, pixel_t color) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            draw_pixel(x + j, y + i, color);
        }
    }
}
/* Simple circle drawing using Midpoint Circle Algorithm */
void graphics_draw_circle(int cx, int cy, int radius, pixel_t color)
{
    int x = radius;
    int y = 0;
    int d = 3 - 2 * radius;

    while (x >= y)
    {
        graphics_draw_point(cx + x, cy + y, color);
        graphics_draw_point(cx - x, cy + y, color);
        graphics_draw_point(cx + x, cy - y, color);
        graphics_draw_point(cx - x, cy - y, color);
        graphics_draw_point(cx + y, cy + x, color);
        graphics_draw_point(cx - y, cy + x, color);
        graphics_draw_point(cx + y, cy - x, color);
        graphics_draw_point(cx - y, cy - x, color);

        if (d < 0)
        {
            d = d + 4 * y + 6;
        }
        else
        {
            d = d + 4 * (y - x) + 10;
            x--;
        }
        y++;
    }
}

void graphics_draw_circle_filled(int cx, int cy, int radius, pixel_t color)
{
    for (int y = -radius; y <= radius; y++)
    {
        for (int x = -radius; x <= radius; x++)
        {
            int dx = x;
            int dy = y;
            if (dx * dx + dy * dy <= radius * radius)
            {
                graphics_draw_point(cx + x, cy + y, color);
            }
        }
    }
}

void graphics_clear(pixel_t color)
{
    graphics_draw_rect_filled(0, 0, g_width, g_height, color);
}

void graphics_fill(pixel_t color)
{
    graphics_draw_rect_filled(0, 0, g_width, g_height, color);
}

/* LEDs
 * This program draws an animation on an LED matrix peripheral.
 *
 * To run this program, make sure that you have instantiated an "LED Matrix"
 * peripheral in the "I/O" tab.
 */
pixel_t rgb(uint8_t r, uint8_t g, uint8_t b) {
    return (((pixel_t)r << 16) | ((pixel_t)g << 8) | (pixel_t)b);
}

pixel_t pixel_from_hex(uint32_t hex)
{
    return hex;
}

uint8_t pixel_get_r(pixel_t pixel)
{
    return (pixel >> 16) & 0xFF;
}

uint8_t pixel_get_g(pixel_t pixel)
{
    return (pixel >> 8) & 0xFF;
}

uint8_t pixel_get_b(pixel_t pixel)
{
    return pixel & 0xFF;
}

void draw_pixel(int x, int y, pixel_t color) {
    if (x >= 0 && x < W && y >= 0 && y < H) {
        *((pixel_t*)LED_MATRIX_0_BASE + (y * W + x)) = color;
    }
}

void draw_rect_filled(int x, int y, int width, int height, pixel_t color) {
    for (int yy = 0; yy < height; yy++) {
        for (int xx = 0; xx < width; xx++) {
            draw_pixel(x + xx, y + yy, color);
        }
    }
}

void init_bricks() {
    for (int r = 0; r < BRICK_ROWS; r++) {
        for (int c = 0; c < BRICK_COLS; c++) {
            bricks[r][c] = 1;
        }
    }
}

void clear_screen() {
    draw_rect(0, 0, W, H, 0x000000);
}


/* ===================================================== */
/* ================= BRICK BREAKER ===================== */
/* ===================================================== */

#define BRICK_ROWS 3
#define BRICK_COLS 7
#define BRICK_W 5
#define BRICK_H 2

uint8_t bricks[BRICK_ROWS][BRICK_COLS];

int paddle_x;
int paddle_w = 6;

int ball_x, ball_y;
int ball_dx, ball_dy;

/* ===== INIT ===== */
void brick_init() {
    paddle_x = W / 2 - 3;

    ball_x = W / 2;
    ball_y = H - 5;
    ball_dx = 1;
    ball_dy = -1;

    // inicializace bricků
    for (int r = 0; r < BRICK_ROWS; r++) {
        for (int c = 0; c < BRICK_COLS; c++) {
            bricks[r][c] = 1;
        }
    }
}

/* ===== STEP ===== */
void brick_step() {
    volatile uint32_t* left  = (uint32_t*)D_PAD_0_LEFT;
    volatile uint32_t* right = (uint32_t*)D_PAD_0_RIGHT;

    /* ---- SMAŽ STARÉ OBJEKTY ---- */
    draw_pixel(ball_x, ball_y, 0x000000);
    draw_rect(paddle_x, H-1, paddle_w, 1, 0x000000);

    /* ---- OVLÁDÁNÍ ---- */
    if (*left && paddle_x > 0) paddle_x--;
    if (*right && paddle_x < W - paddle_w) paddle_x++;

    /* ---- POHYB MÍČKU ---- */
    ball_x += ball_dx;
    ball_y += ball_dy;

    /* ---- ODRAZY OD STĚN ---- */
    if (ball_x <= 0 || ball_x >= W-1) ball_dx *= -1;
    if (ball_y <= 0) ball_dy *= -1;

    /* ---- ODRAZ OD PÁLKY ---- */
    if (ball_y == H-2 &&
        ball_x >= paddle_x &&
        ball_x < paddle_x + paddle_w) {
        ball_dy *= -1;
    }

    /* ---- KOLIZE S BRICKY ---- */
    if (ball_y < BRICK_ROWS * (BRICK_H + 1)) {

        int r = ball_y / (BRICK_H + 1);
        int c = ball_x / BRICK_W;

        if (r >= 0 && r < BRICK_ROWS &&
            c >= 0 && c < BRICK_COLS) {

            if (bricks[r][c]) {
                bricks[r][c] = 0;   // znič cihlu
                ball_dy *= -1;      // odraz
            }
        }
    }

    /* ---- PÁD MÍČKU ---- */
    if (ball_y >= H) {
        brick_init();
        clear_screen();
        return;
    }

    /* ---- VYKRESLENÍ ---- */

    // pálka
    draw_rect(paddle_x, H-1, paddle_w, 1, 0x00FF00);

    // míček
    draw_pixel(ball_x, ball_y, 0xFFFFFF);

    // bricks
    for (int r = 0; r < BRICK_ROWS; r++) {
        for (int c = 0; c < BRICK_COLS; c++) {
            if (bricks[r][c]) {
                draw_rect(
                    c * BRICK_W,
                    r * (BRICK_H + 1),
                    BRICK_W - 1,
                    BRICK_H,
                    0xFF0000
                );
            }
        }
    }
}
/* ===================================================== */
/* ====================== SNAKE ========================= */
/* ===================================================== */

#define MAX_LEN 50

int snake_x[MAX_LEN];
int snake_y[MAX_LEN];
int snake_len = 5;

int dir_x = 1, dir_y = 0;

int food_x = 10, food_y = 10;

void snake_init() {
    snake_len = 5;
    for (int i = 0; i < snake_len; i++) {
        snake_x[i] = 5 - i;
        snake_y[i] = 5;
    }
    dir_x = 1; dir_y = 0;
    food_x = 15;
    food_y = 10;
}

void snake_step() {
    volatile uint32_t* up    = (uint32_t*)D_PAD_0_UP;
    volatile uint32_t* down  = (uint32_t*)D_PAD_0_DOWN;
    volatile uint32_t* left  = (uint32_t*)D_PAD_0_LEFT;
    volatile uint32_t* right = (uint32_t*)D_PAD_0_RIGHT;

    // ovládání
    if (*up)    { dir_x = 0; dir_y = -1; }
    if (*down)  { dir_x = 0; dir_y = 1; }
    if (*left)  { dir_x = -1; dir_y = 0; }
    if (*right) { dir_x = 1; dir_y = 0; }

    // smaž ocas
    draw_pixel(snake_x[snake_len-1], snake_y[snake_len-1], 0x000000);

    // posun těla
    for (int i = snake_len-1; i > 0; i--) {
        snake_x[i] = snake_x[i-1];
        snake_y[i] = snake_y[i-1];
    }

    // hlava
    snake_x[0] += dir_x;
    snake_y[0] += dir_y;

    // kolize se stěnou
    if (snake_x[0] < 0 || snake_x[0] >= W || snake_y[0] < 0 || snake_y[0] >= H) {
        snake_init();
        clear_screen();
        return;
    }

    // jídlo
    if (snake_x[0] == food_x && snake_y[0] == food_y) {
        if (snake_len < MAX_LEN) snake_len++;
        food_x = (food_x + 7) % W;
        food_y = (food_y + 5) % H;
    }

    // vykreslení
    for (int i = 0; i < snake_len; i++)
        draw_pixel(snake_x[i], snake_y[i], 0x00FF00);

    draw_pixel(food_x, food_y, 0xFF0000);
}

/* ===================================================== */
/* ======================= MAIN ========================= */
/* ===================================================== */

int main() {
    volatile uint32_t* switches = (uint32_t*)SWITCHES_0_BASE;

    int last_mode = -1;

    while (1) {
        int mode = (*switches) & 0x1;

        // změna hry → reset
        if (mode != last_mode) {
            clear_screen();

            if (mode == 0) brick_init();
            else snake_init();

            last_mode = mode;
        }

        // běh hry
        if (mode == 0) {
            brick_step();
        } else {
            snake_step();
        }

        // delay
        for (volatile int i = 0; i < 500; i++);
    }
}