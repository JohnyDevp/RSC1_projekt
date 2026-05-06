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

#define BRICK_ROWS 5
#define BRICK_COLS 10
#define BRICK_W (W / BRICK_COLS)
#define BRICK_H 3

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

void draw_pixel(int x, int y, pixel_t color) {
    if (x >= 0 && x < W && y >= 0 && y < H) {
        *((pixel_t*)LED_MATRIX_0_BASE + (y * W + x)) = color;
    }
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



/* ==== GAME STATE ==== */
int paddle_x;
int paddle_w = 24;

int ball_x, ball_y;
int ball_dx, ball_dy;

/* ==== OLD POSITIONS (KLÍČOVÉ!) ==== */
int old_ball_x, old_ball_y;
int old_paddle_x;

/* ==== INIT ==== */
void brick_init() {
    paddle_x = W/2 - paddle_w/2;
    old_paddle_x = paddle_x;

    ball_x = W/2;
    ball_y = H - 6;
    ball_dx = 1;
    ball_dy = -1;

    old_ball_x = ball_x;
    old_ball_y = ball_y;

    clear_screen();

    /* vytvoř a vykresli bricky */
    for (int r = 0; r < BRICK_ROWS; r++) {
        for (int c = 0; c < BRICK_COLS; c++) {
            bricks[r][c] = 1;

            draw_rect_filled(
                c * BRICK_W,
                r * BRICK_H,
                BRICK_W - 1,
                BRICK_H - 1,
                0xFF0000
            );
        }
    }

    /* první vykreslení */
    draw_rect_filled(paddle_x, H-2, paddle_w, 2, 0x00FF00);
    draw_pixel(ball_x, ball_y, 0xFFFFFF);
}

/* ==== STEP ==== */
void brick_step() {
    volatile uint32_t* left  = (uint32_t*)D_PAD_0_LEFT;
    volatile uint32_t* right = (uint32_t*)D_PAD_0_RIGHT;

    /* ================= MAZÁNÍ ================= */

    /* smaž starý míček */
    draw_pixel(old_ball_x, old_ball_y, 0x000000);

    /* smaž pálku jen pokud se hnula */
    draw_rect_filled(old_paddle_x, H-2, paddle_w, 2, 0x000000);

    /* ================= OVLÁDÁNÍ ================= */

    if (*left && paddle_x > 0) paddle_x--;
    if (*right && paddle_x < W - paddle_w) paddle_x++;

    /* ================= POHYB ================= */

    ball_x += ball_dx;
    ball_y += ball_dy;

    /* stěny */
    if (ball_x <= 0 || ball_x >= W-1) ball_dx *= -1;
    if (ball_y <= 0) ball_dy *= -1;

    /* pálka */
    if (ball_y >= H-3 &&
        ball_x >= paddle_x &&
        ball_x < paddle_x + paddle_w) {

if (ball_y >= H-3 && ball_x >= paddle_x && ball_x < paddle_x + paddle_w) {
    int hit = ball_x - paddle_x;
    
    // Rozdělíme pálku na 3 zóny:
    if (hit < paddle_w / 3) {
        ball_dx = -1; // Vždy vlevo
    } 
    else if (hit > (2 * paddle_w) / 3) {
        ball_dx = 1;  // Vždy vpravo
    }
    // Ve střední části (hit mezi 1/3 a 2/3) dx neměníme! 
    // Míček se odrazí podle toho, pod jakým úhlem přiletěl.

    ball_dy = -1; // Odraz nahoru
}
    }

    /* ================= BRICK KOLIZE ================= */

    int r = ball_y / BRICK_H;
    int c = ball_x / BRICK_W;

    if (r >= 0 && r < BRICK_ROWS &&
        c >= 0 && c < BRICK_COLS &&
        bricks[r][c]) {

        bricks[r][c] = 0;

        draw_rect_filled(
            c * BRICK_W,
            r * BRICK_H,
            BRICK_W - 1,
            BRICK_H - 1,
            0x000000
        );

        ball_dy *= -1;
    }

    /* ================= RESET ================= */

    if (ball_y >= H) {
        brick_init();
        return;
    }

    /* ================= KRESLENÍ ================= */

    draw_rect_filled(paddle_x, H-2, paddle_w, 2, 0x00FF00);
    draw_pixel(ball_x, ball_y, 0xFFFFFF);

    /* ================= ULOŽ POZICE ================= */

    old_ball_x = ball_x;
    old_ball_y = ball_y;
    old_paddle_x = paddle_x;
}



/* ===================================================== */
/* ======================= SNAKE ========================= */
/* ===================================================== */

#define MAX_LEN 60
#define OBST_COUNT 10
#define CELL 2

int snake_x[MAX_LEN];
int snake_y[MAX_LEN];
int snake_len;

int dir_x, dir_y;

int food_x, food_y;     // +1
int bonus_x, bonus_y;   // +2
int bonus_timer;

int obst_x[OBST_COUNT];
int obst_y[OBST_COUNT];

/* ================= DRAW BLOCK ================= */

void draw_block(int x, int y, uint32_t color) {
    for (int yy = 0; yy < CELL; yy++)
        for (int xx = 0; xx < CELL; xx++)
            draw_pixel(x + xx, y + yy, color);
}

/* ================= HELPERS ================= */

int is_obstacle(int x, int y) {
    for (int i = 0; i < OBST_COUNT; i++)
        if (obst_x[i] == x && obst_y[i] == y)
            return 1;
    return 0;
}

int is_food(int x, int y) {
    return (x == food_x && y == food_y);
}

int is_bonus(int x, int y) {
    return (x == bonus_x && y == bonus_y);
}

/* správné mazání */
void erase_tail(int x, int y) {
    if (is_obstacle(x, y)) {
        draw_block(x, y, 0x888888);
    }
    else if (is_food(x, y)) {
        draw_block(x, y, 0xFF0000);
    }
    else if (is_bonus(x, y)) {
        draw_block(x, y, 0xFFFF00);
    }
    else {
        draw_block(x, y, 0x000000);
    }
}

/* ================= INIT ================= */

void snake_init() {
    clear_screen();

    snake_len = 5;
    dir_x = 1;
    dir_y = 0;

    food_x  = (20 % (W / CELL)) * CELL;
    food_y  = (15 % (H / CELL)) * CELL;

    bonus_x = (30 % (W / CELL)) * CELL;
    bonus_y = (20 % (H / CELL)) * CELL;
    bonus_timer = 0;

    /* překážky */
    for (int i = 0; i < OBST_COUNT; i++) {
        obst_x[i] = ((i * 11) % (W / CELL)) * CELL;
        obst_y[i] = ((i * 7) % (H / CELL)) * CELL;
    }

    /* had */
    for (int i = 0; i < snake_len; i++) {
        snake_x[i] = (10 - i) * CELL;
        snake_y[i] = 10 * CELL;
    }

    /* vykreslení */
    for (int i = 0; i < OBST_COUNT; i++)
        draw_block(obst_x[i], obst_y[i], 0x888888);

    draw_block(food_x, food_y, 0xFF0000);
    draw_block(bonus_x, bonus_y, 0xFFFF00);

    for (int i = 0; i < snake_len; i++)
        draw_block(snake_x[i], snake_y[i], 0x00FF00);
}

/* ================= STEP ================= */

void snake_step() {
    volatile uint32_t* up    = (uint32_t*)D_PAD_0_UP;
    volatile uint32_t* down  = (uint32_t*)D_PAD_0_DOWN;
    volatile uint32_t* left  = (uint32_t*)D_PAD_0_LEFT;
    volatile uint32_t* right = (uint32_t*)D_PAD_0_RIGHT;

    /* ovládání */
    if (*up && dir_y != 1)    { dir_x = 0; dir_y = -1; }
    if (*down && dir_y != -1) { dir_x = 0; dir_y = 1; }
    if (*left && dir_x != 1)  { dir_x = -1; dir_y = 0; }
    if (*right && dir_x != -1){ dir_x = 1; dir_y = 0; }

    /* smaž ocas */
    erase_tail(snake_x[snake_len-1], snake_y[snake_len-1]);

    /* posun těla */
    for (int i = snake_len-1; i > 0; i--) {
        snake_x[i] = snake_x[i-1];
        snake_y[i] = snake_y[i-1];
    }

    /* hlava */
    snake_x[0] += dir_x * CELL;
    snake_y[0] += dir_y * CELL;

    /* kolize stěna */
    if (snake_x[0] < 0 || snake_x[0] >= W ||
        snake_y[0] < 0 || snake_y[0] >= H) {
        snake_init();
        return;
    }

    /* self collision */
    for (int i = 1; i < snake_len; i++) {
        if (snake_x[0] == snake_x[i] &&
            snake_y[0] == snake_y[i]) {
            snake_init();
            return;
        }
    }

    /* překážky */
    for (int i = 0; i < OBST_COUNT; i++) {
        if (snake_x[0] == obst_x[i] &&
            snake_y[0] == obst_y[i]) {
            snake_init();
            return;
        }
    }

    /* ===== FOOD (+1) ===== */
    if (snake_x[0] == food_x && snake_y[0] == food_y) {

        if (snake_len < MAX_LEN) snake_len += 1;

        draw_block(food_x, food_y, 0x000000);

        do {
            food_x = (((food_x / CELL) + 13) % (W / CELL)) * CELL;
            food_y = (((food_y / CELL) + 9) % (H / CELL)) * CELL;
        } while (is_obstacle(food_x, food_y));
    }

    /* ===== BONUS (+2) ===== */
    bonus_timer++;

    if (bonus_timer > 200) {
        draw_block(bonus_x, bonus_y, 0x000000);

        bonus_x = (((bonus_x / CELL) + 17) % (W / CELL)) * CELL;
        bonus_y = (((bonus_y / CELL) + 5) % (H / CELL)) * CELL;

        bonus_timer = 0;
    }

    if (snake_x[0] == bonus_x && snake_y[0] == bonus_y) {

        if (snake_len < MAX_LEN - 2) snake_len += 2;

     
        draw_block(bonus_x, bonus_y, 0x000000);

        do {
            bonus_x = (((bonus_x / CELL) + 17) % (W / CELL)) * CELL;
            bonus_y = (((bonus_y / CELL) + 5) % (H / CELL)) * CELL;
        } while (is_obstacle(bonus_x, bonus_y));

        bonus_timer = 0;
    }

    /* ===== KRESLENÍ ===== */

    draw_block(snake_x[0], snake_y[0], 0x00FF00);

    draw_block(food_x, food_y, 0xFF0000);
    draw_block(bonus_x, bonus_y, 0xFFFF00);

    for (int i = 0; i < OBST_COUNT; i++)
        draw_block(obst_x[i], obst_y[i], 0x888888);
}
/* ===================================================== */
/* ======================= MAIN ========================= */
/* ===================================================== */

int main() {
    volatile uint32_t* switches = (uint32_t*)SWITCHES_0_BASE;

    int last_mode = -1;

    clear_screen();

    while (1) {

        int mode = (*switches) & 1;

        /* ================= SWITCH GAME ================= */
        if (mode != last_mode) {

            clear_screen();

            if (mode == 0) {
                brick_init();
            } else {
                snake_init();
            }

            last_mode = mode;
        }

        /* ================= RUN GAME ================= */

        if (mode == 0) {
            brick_step();
        } else {
            snake_step();
        }

        /* delay */
        for (volatile int i = 0; i <600; i++);
    }
}
