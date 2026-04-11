CC=/home/johnny/Downloads/riscv64-unknown-elf-gcc-8.3.0-2020.04.1-x86_64-linux-centos6/bin/riscv64-unknown-elf-gcc
AR=/home/johnny/Downloads/riscv64-unknown-elf-gcc-8.3.0-2020.04.1-x86_64-linux-centos6/bin/riscv64-unknown-elf-ar
CFLAGS=-march=rv32imc -mabi=ilp32 -O0 -g
LDFLAGS=-static -lm

# ************************ EXECUTABLES ************************

# make using library
all-lib: lib main.o
	${CC} ${CFLAGS} build/bins/main.o -o build/main.elf -Lbuild/bins -lgfx ${LDFLAGS}

# make without library, just binaries
all: pixel.o graphics.o main.o
	${CC} ${CFLAGS} build/bins/main.o build/bins/graphics.o build/bins/pixel.o -o build/main.elf ${LDFLAGS}

all-tests: test_draw_rect test_init_getters test_control_printout test_draw_point test_draw_line test_draw_rect_filled test_draw_circle test_draw_circle_filled test_clear test_fill

test_draw_rect: src/test_draw_rect.c graphics.o pixel.o
	${CC} ${CFLAGS} src/test_draw_rect.c build/bins/graphics.o build/bins/pixel.o -o build/test_draw_rect.elf ${LDFLAGS}

test_init_getters: src/test_init_getters.c graphics.o pixel.o
	${CC} ${CFLAGS} src/test_init_getters.c build/bins/graphics.o build/bins/pixel.o -o build/test_init_getters.elf ${LDFLAGS}

test_control_printout: src/test_control_printout.c graphics.o pixel.o
	${CC} ${CFLAGS} src/test_control_printout.c build/bins/graphics.o build/bins/pixel.o -o build/test_control_printout.elf ${LDFLAGS}

test_draw_point: src/test_draw_point.c graphics.o pixel.o
	${CC} ${CFLAGS} src/test_draw_point.c build/bins/graphics.o build/bins/pixel.o -o build/test_draw_point.elf ${LDFLAGS}

test_draw_line: src/test_draw_line.c graphics.o pixel.o
	${CC} ${CFLAGS} src/test_draw_line.c build/bins/graphics.o build/bins/pixel.o -o build/test_draw_line.elf ${LDFLAGS}

test_draw_rect_filled: src/test_draw_rect_filled.c graphics.o pixel.o
	${CC} ${CFLAGS} src/test_draw_rect_filled.c build/bins/graphics.o build/bins/pixel.o -o build/test_draw_rect_filled.elf ${LDFLAGS}

test_draw_circle: src/test_draw_circle.c graphics.o pixel.o
	${CC} ${CFLAGS} src/test_draw_circle.c build/bins/graphics.o build/bins/pixel.o -o build/test_draw_circle.elf ${LDFLAGS}

test_draw_circle_filled: src/test_draw_circle_filled.c graphics.o pixel.o
	${CC} ${CFLAGS} src/test_draw_circle_filled.c build/bins/graphics.o build/bins/pixel.o -o build/test_draw_circle_filled.elf ${LDFLAGS}

test_clear: src/test_clear.c graphics.o pixel.o
	${CC} ${CFLAGS} src/test_clear.c build/bins/graphics.o build/bins/pixel.o -o build/test_clear.elf ${LDFLAGS}

test_fill: src/test_fill.c graphics.o pixel.o
	${CC} ${CFLAGS} src/test_fill.c build/bins/graphics.o build/bins/pixel.o -o build/test_fill.elf ${LDFLAGS}
# ************************ BINARIES ************************

graphics.o: src/lib/graphics.c src/lib/graphics.h
	${CC} ${CFLAGS} -c src/lib/graphics.c -o build/bins/graphics.o

pixel.o: src/lib/pixel.c src/lib/pixel.h
	${CC} ${CFLAGS} -c src/lib/pixel.c -o build/bins/pixel.o

main.o: src/main.c src/lib/graphics.h src/lib/pixel.h
	${CC} ${CFLAGS} -c src/main.c -o build/bins/main.o
	
# ************************ LIBRARY ************************

lib: graphics.o pixel.o
	${AR} rcs build/bins/libgfx.a build/bins/graphics.o build/bins/pixel.o

clean:
	rm -f build/bins/*.elf build/bins/*.o build/bins/*.a build/*.elf
