# RSC1 Graphics Library Project

This project contains a small graphics library for RISC-V, a demo program, and automated test programs that run in Ripes CLI.

## Project layout

- src/lib/graphics.c, src/lib/graphics.h: drawing functions
- src/lib/pixel.c, src/lib/pixel.h: color and pixel helpers
- src/main.c: main demo program
- tests/tests.c: test source with 10 selectable test cases
- Makefile: build and run commands
- build/bins: object files and static library output
- build/tests: generated test executables

## Requirements

- A RISC-V GCC toolchain with gcc and ar
- Ripes CLI binary
- make

Current Makefile paths are set to:

- Compiler: /home/johnny/Downloads/riscv64-unknown-elf-gcc-8.3.0-2020.04.1-x86_64-linux-centos6/bin/riscv64-unknown-elf-gcc
- Archiver: /home/johnny/Downloads/riscv64-unknown-elf-gcc-8.3.0-2020.04.1-x86_64-linux-centos6/bin/riscv64-unknown-elf-ar
- Ripes: /home/johnny/Programs/Ripes/Ripes

If your tools are in a different location, update CC, AR, and RIPES in Makefile.

## Build commands

- Build demo executable by compiling objects directly:

    make all

    Output: build/main.elf

- Build demo executable and link using static library:

    make all-lib

    Output: build/main.elf

- Build static graphics library only:

    make lib

    Output: build/bins/libgfx.a

- Clean build artifacts:

    make clean

## Test system

Tests are defined in tests/tests.c and selected by TEST_CASE at compile time.

Implemented cases:

1. graphics_init, graphics_get_width, graphics_get_height
2. control_printout
3. graphics_draw_point
4. graphics_draw_line
5. graphics_draw_rect
6. graphics_draw_rect_filled
7. graphics_draw_circle
8. graphics_draw_circle_filled
9. graphics_clear
10. graphics_fill

### Build all test executables

make test

This generates:

- build/tests/test_1.elf
- build/tests/test_2.elf
- build/tests/test_3.elf
- build/tests/test_4.elf
- build/tests/test_5.elf
- build/tests/test_6.elf
- build/tests/test_7.elf
- build/tests/test_8.elf
- build/tests/test_9.elf
- build/tests/test_10.elf

### Run one test in Ripes CLI

make run-test-1

You can replace 1 with any test number from 1 to 10.

### Run all tests in Ripes CLI

make run-tests

The run stops at the first failing test.

## Notes

- Compilation target is rv32imc with ilp32 ABI.
- Ripes run targets use processor RV32_5S with ISA extensions M,C.
- Test timeout is configured in Makefile with RIPES_TIMEOUT.
