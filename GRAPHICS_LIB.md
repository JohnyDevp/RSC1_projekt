# Graphics Library for LED Matrix (RISC-V)

A simple, efficient graphics library for drawing on LED matrix displays in RISC-V systems.

## Files

### Core Library

- **`graphics.h`** / **`graphics.c`**: Main graphics API and drawing algorithms
- **`pixel.h`** / **`pixel.c`**: Color and pixel utilities

### Application

- **`main.c`**: Example usage with LED matrix animation

## API Reference

### Initialization

```c
void graphics_init(unsigned* led_base, int width, int height);
```

Initialize the graphics library with LED matrix address and dimensions.

### Drawing Functions

#### Basic Operations

```c
void graphics_draw_point(int x, int y, pixel_t color);
```

Draw a single pixel at (x, y).

```c
void graphics_draw_line(int x0, int y0, int x1, int y1, pixel_t color);
```

Draw a line using Bresenham's algorithm.

```c
void graphics_draw_rect(int x, int y, int width, int height, pixel_t color);
void graphics_draw_rect_filled(int x, int y, int width, int height, pixel_t color);
```

Draw rectangle outline or filled rectangle.

```c
void graphics_draw_circle(int cx, int cy, int radius, pixel_t color);
void graphics_draw_circle_filled(int cx, int cy, int radius, pixel_t color);
```

Draw circle outline or filled circle using Midpoint Circle Algorithm.

### Utility Functions

```c
void graphics_clear(pixel_t color);
void graphics_fill(pixel_t color);
int graphics_get_width(void);
int graphics_get_height(void);
```

### Color Functions

```c
pixel_t pixel_rgb(uint8_t r, uint8_t g, uint8_t b);
pixel_t pixel_from_hex(uint32_t hex);
uint8_t pixel_get_r(pixel_t pixel);
uint8_t pixel_get_g(pixel_t pixel);
uint8_t pixel_get_b(pixel_t pixel);
```

## Compilation

```bash
riscv64-unknown-elf-gcc -c pixel.c -o pixel.o
riscv64-unknown-elf-gcc -c graphics.c -o graphics.o
riscv64-unknown-elf-gcc -c main.c -o main.o
riscv64-unknown-elf-gcc pixel.o graphics.o main.o -o program
```

## Example Usage

```c
#include "graphics.h"
#include "pixel.h"

// In main()
graphics_init(led_base, 35, 25);

// Draw a red pixel
graphics_draw_point(10, 10, pixel_rgb(255, 0, 0));

// Draw a green line
graphics_draw_line(0, 0, 20, 20, pixel_rgb(0, 255, 0));

// Draw a blue filled circle
graphics_draw_circle_filled(17, 12, 5, pixel_rgb(0, 0, 255));

// Clear screen with black
graphics_clear(pixel_rgb(0, 0, 0));
```

## Design Notes

- **Single global state**: Graphics state is stored statically (width, height, LED base address)
- **Clipping**: Out-of-bounds pixels are silently ignored
- **Colors**: 24-bit RGB format (8 bits per channel) stored in 32-bit `pixel_t`
- **Algorithms**:
    - Line: Bresenham's algorithm (efficient, integer-only)
    - Circle: Midpoint Circle Algorithm (efficient rasterization)
