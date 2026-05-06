#include <stdint.h>
#include <stdio.h>

#define W LED_MATRIX_0_WIDTH
#define H LED_MATRIX_0_HEIGHT

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

#include "lib/graphics.h"
#include "lib/pixel.h"

/* ===================================================== */
/* ================= BRICK BREAKER ===================== */
/* ===================================================== */

#define BRICK_ROWS 5
#define BRICK_COLS 10
#define BRICK_W (W / BRICK_COLS)
#define BRICK_H 3

uint8_t bricks[BRICK_ROWS][BRICK_COLS];

/* ==== GAME STATE ==== */
int paddle_x;
int paddle_w = 24;

int ball_x, ball_y;
int ball_dx, ball_dy;

/* ==== OLD POSITIONS (KLÍČOVÉ!) ==== */
int old_ball_x, old_ball_y;
int old_paddle_x;

/* ==== INIT ==== */
void brick_init()
{
    paddle_x = W / 2 - paddle_w / 2;
    old_paddle_x = paddle_x;

    ball_x = W / 2;
    ball_y = H - 6;
    ball_dx = 1;
    ball_dy = -1;

    old_ball_x = ball_x;
    old_ball_y = ball_y;

    graphics_clear_screen();

    /* vytvoř a vykresli bricky */
    for (int r = 0; r < BRICK_ROWS; r++)
    {
        for (int c = 0; c < BRICK_COLS; c++)
        {
            bricks[r][c] = 1;

            graphics_draw_rect_filled(
                c * BRICK_W,
                r * BRICK_H,
                BRICK_W - 1,
                BRICK_H - 1,
                0xFF0000);
        }
    }

    /* první vykreslení */
    graphics_draw_rect_filled(paddle_x, H - 2, paddle_w, 2, 0x00FF00);
    graphics_draw_pixel(ball_x, ball_y, 0xFFFFFF);
}

/* ==== STEP ==== */
void brick_step()
{
    volatile uint32_t *left = (uint32_t *)D_PAD_0_LEFT;
    volatile uint32_t *right = (uint32_t *)D_PAD_0_RIGHT;

    /* ================= MAZÁNÍ ================= */

    /* smaž starý míček */
    graphics_draw_pixel(old_ball_x, old_ball_y, 0x000000);

    /* smaž pálku jen pokud se hnula */
    graphics_draw_rect_filled(old_paddle_x, H - 2, paddle_w, 2, 0x000000);

    /* ================= OVLÁDÁNÍ ================= */

    if (*left && paddle_x > 0)
        paddle_x--;
    if (*right && paddle_x < W - paddle_w)
        paddle_x++;

    /* ================= POHYB ================= */

    ball_x += ball_dx;
    ball_y += ball_dy;

    /* stěny */
    if (ball_x <= 0 || ball_x >= W - 1)
        ball_dx *= -1;
    if (ball_y <= 0)
        ball_dy *= -1;

    /* pálka */
    if (ball_y >= H - 3 &&
        ball_x >= paddle_x &&
        ball_x < paddle_x + paddle_w)
    {

        int hit = ball_x - paddle_x;

        // Rozdělíme pálku na 3 zóny:
        if (hit < paddle_w / 3)
        {
            ball_dx = -1; // Vždy vlevo
        }
        else if (hit > (2 * paddle_w) / 3)
        {
            ball_dx = 1; // Vždy vpravo
        }
        // Ve střední části dx neměníme

        ball_dy = -1; // Odraz nahoru
    }

    /* ================= BRICK KOLIZE ================= */

    int r = ball_y / BRICK_H;
    int c = ball_x / BRICK_W;

    if (r >= 0 && r < BRICK_ROWS &&
        c >= 0 && c < BRICK_COLS &&
        bricks[r][c])
    {

        bricks[r][c] = 0;

        graphics_draw_rect_filled(
            c * BRICK_W,
            r * BRICK_H,
            BRICK_W - 1,
            BRICK_H - 1,
            0x000000);

        ball_dy *= -1;
    }

    /* ================= RESET ================= */

    if (ball_y >= H)
    {
        brick_init();
        return;
    }

    /* ================= KRESLENÍ ================= */

    graphics_draw_rect_filled(paddle_x, H - 2, paddle_w, 2, 0x00FF00);
    graphics_draw_pixel(ball_x, ball_y, 0xFFFFFF);

    /* ================= ULOŽ POZICE ================= */

    old_ball_x = ball_x;
    old_ball_y = ball_y;
    old_paddle_x = paddle_x;
}

void init_bricks()
{
    for (int r = 0; r < BRICK_ROWS; r++)
    {
        for (int c = 0; c < BRICK_COLS; c++)
        {
            bricks[r][c] = 1;
        }
    }
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

int food_x, food_y;
int bonus_x, bonus_y;
int bonus_timer;

int obst_x[OBST_COUNT];
int obst_y[OBST_COUNT];

/* ================= HELPERS ================= */

int is_obstacle(int x, int y)
{
    for (int i = 0; i < OBST_COUNT; i++)
        if (obst_x[i] == x && obst_y[i] == y)
            return 1;
    return 0;
}

int is_food(int x, int y)
{
    return (x == food_x && y == food_y);
}

int is_bonus(int x, int y)
{
    return (x == bonus_x && y == bonus_y);
}

/* draw a CELL x CELL block */
void draw_block(int x, int y, uint32_t color)
{
    for (int yy = 0; yy < CELL; yy++)
        for (int xx = 0; xx < CELL; xx++)
            graphics_draw_pixel(x + xx, y + yy, color);
}

/* KLÍČOVÁ FUNKCE – správné mazání */
void erase_tail(int x, int y)
{
    if (is_obstacle(x, y))
    {
        draw_block(x, y, 0x888888);
    }
    else if (is_food(x, y))
    {
        draw_block(x, y, 0xFF0000);
    }
    else if (is_bonus(x, y))
    {
        draw_block(x, y, 0xFFFF00);
    }
    else
    {
        draw_block(x, y, 0x000000);
    }
}

/* ================= INIT ================= */

void snake_init()
{
    graphics_clear_screen(); // důležité

    snake_len = 5;

    dir_x = 1;
    dir_y = 0;

    food_x = (20 % (W / CELL)) * CELL;
    food_y = (15 % (H / CELL)) * CELL;

    bonus_x = (30 % (W / CELL)) * CELL;
    bonus_y = (20 % (H / CELL)) * CELL;
    bonus_timer = 0;

    /* překážky */
    for (int i = 0; i < OBST_COUNT; i++)
    {
        obst_x[i] = ((i * 11) % (W / CELL)) * CELL;
        obst_y[i] = ((i * 7) % (H / CELL)) * CELL;
    }

    /* had */
    for (int i = 0; i < snake_len; i++)
    {
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

void snake_step()
{
    volatile uint32_t *up = (uint32_t *)D_PAD_0_UP;
    volatile uint32_t *down = (uint32_t *)D_PAD_0_DOWN;
    volatile uint32_t *left = (uint32_t *)D_PAD_0_LEFT;
    volatile uint32_t *right = (uint32_t *)D_PAD_0_RIGHT;

    /* ovládání */
    if (*up && dir_y != 1)
    {
        dir_x = 0;
        dir_y = -1;
    }
    if (*down && dir_y != -1)
    {
        dir_x = 0;
        dir_y = 1;
    }
    if (*left && dir_x != 1)
    {
        dir_x = -1;
        dir_y = 0;
    }
    if (*right && dir_x != -1)
    {
        dir_x = 1;
        dir_y = 0;
    }

    /* smaž ocas SPRÁVNĚ */
    erase_tail(snake_x[snake_len - 1], snake_y[snake_len - 1]);

    /* posun těla */
    for (int i = snake_len - 1; i > 0; i--)
    {
        snake_x[i] = snake_x[i - 1];
        snake_y[i] = snake_y[i - 1];
    }

    /* hlava */
    snake_x[0] += dir_x * CELL;
    snake_y[0] += dir_y * CELL;

    /* kolize stěny */
    if (snake_x[0] < 0 || snake_x[0] >= W ||
        snake_y[0] < 0 || snake_y[0] >= H)
    {
        graphics_clear_screen();
        snake_init();
        return;
    }

    /* self collision */
    for (int i = 1; i < snake_len; i++)
    {
        if (snake_x[0] == snake_x[i] &&
            snake_y[0] == snake_y[i])
        {
            graphics_clear_screen();
            snake_init();
            return;
        }
    }

    /* překážky */
    for (int i = 0; i < OBST_COUNT; i++)
    {
        if (snake_x[0] == obst_x[i] &&
            snake_y[0] == obst_y[i])
        {
            graphics_clear_screen();
            snake_init();
            return;
        }
    }

    /* ===== FOOD (+1) ===== */
    if (snake_x[0] == food_x && snake_y[0] == food_y)
    {

        if (snake_len < MAX_LEN)
            snake_len += 1;

        draw_block(food_x, food_y, 0x000000);

        do
        {
            food_x = (((food_x / CELL) + 13) % (W / CELL)) * CELL;
            food_y = (((food_y / CELL) + 9) % (H / CELL)) * CELL;
        } while (is_obstacle(food_x, food_y));
    }

    /* ===== BONUS (+2) ===== */
    bonus_timer++;

    if (bonus_timer > 200)
    {
        draw_block(bonus_x, bonus_y, 0x000000);

        bonus_x = (((bonus_x / CELL) + 17) % (W / CELL)) * CELL;
        bonus_y = (((bonus_y / CELL) + 5) % (H / CELL)) * CELL;

        bonus_timer = 0;
    }

    if (snake_x[0] == bonus_x && snake_y[0] == bonus_y)
    {

        if (snake_len < MAX_LEN - 2)
            snake_len += 2;

        draw_block(bonus_x, bonus_y, 0x000000);

        do
        {
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

int main()
{
    volatile uint32_t *switches = (uint32_t *)SWITCHES_0_BASE;

    uint32_t last_mode = (*switches & 0xFF);

    graphics_init((unsigned *)LED_MATRIX_0_BASE, W, H);

    graphics_clear_screen();

    while (1)
    {

        uint32_t mode = (*switches) & 0xFF;
        printf("Mode: %u\n", mode);

        /* ================= SWITCH GAME ================= */
        if (mode != last_mode)
        {

            if (mode == 1)
            {
                graphics_clear_screen();
                brick_init();
            }
            else if (mode == 2)
            {
                graphics_clear_screen();
                snake_init();
            }
            else if (mode == 4)
            {
                // draw pixel test
                for (int i = 0; i < 10; i++)
                {
                    graphics_draw_point(i, i, 0xFF0000);
                    graphics_draw_point(W - 1 - i, i, 0x00FF00);
                }
            }
            else if (mode == 8)
            {
                // draw rect test
                graphics_draw_rect(10, 10, 50, 30, 0x0000FF);
            }
            else if (mode == 16)
            {
                // draw rect filled test
                graphics_draw_rect_filled(20, 20, 50, 30, 0xFFFF00);
            }
            else if (mode == 32)
            {
                // draw circle test
                graphics_draw_circle(W / 2 - 20, H / 2 - 20, 20, 0xFF00FF);
                graphics_draw_circle_filled(W / 2, H / 2, 20, 0x00FFFF);
            }
            else if (mode == 64)
            {
                // draw random image
                const uint8_t img_gray[4 * 4] = {
                    0, 64, 128, 255,
                    255, 128, 64, 0,
                    32, 96, 160, 224,
                    224, 160, 96, 32};

                graphics_draw_image_u8(img_gray, 4, 4, 1, 50, 20);
            }
            else if (mode == 128)
            {
                graphics_clear_screen();
            }

            last_mode = mode;
        }

        /* ================= RUN GAME ================= */

        if (mode == 1)
        {
            brick_step();
        }
        else if (mode == 2)
        {
            snake_step();
        }

        /* delay */
        for (volatile int i = 0; i < 800; i++)
            ;
    }
}

