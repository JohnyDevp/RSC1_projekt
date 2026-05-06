CC=/home/johnny/Downloads/riscv64-unknown-elf-gcc-8.3.0-2020.04.1-x86_64-linux-centos6/bin/riscv64-unknown-elf-gcc
AR=/home/johnny/Downloads/riscv64-unknown-elf-gcc-8.3.0-2020.04.1-x86_64-linux-centos6/bin/riscv64-unknown-elf-ar
CFLAGS=-march=rv32im -mabi=ilp32 -O0 -g
LDFLAGS=-static -lm

# ************************ EXECUTABLES ************************

all: main main-lib benchmark of-main of-benchmark of-img of-img_fullwidth

# make using library
main-lib: lib main.o
	${CC} ${CFLAGS} build/bin/main.o -o build/main.elf -Lbuild/bin -lgfx ${LDFLAGS}

# make without library, just binaries
main: pixel.o graphics.o main.o
	${CC} ${CFLAGS} build/bin/main.o build/bin/graphics.o build/bin/pixel.o -o build/main.elf ${LDFLAGS}

of-main: src/onefile/main.c
	${CC} ${CFLAGS} -x c src/onefile/main.c -o build/of-main.elf ${LDFLAGS}

of-benchmark: src/onefile/benchmark.c
	${CC} ${CFLAGS} -x c src/onefile/benchmark.c -o build/of-benchmark.elf ${LDFLAGS}

of-img: src/onefile/img.c
	${CC} ${CFLAGS} -x c src/onefile/img.c -o build/of-img.elf ${LDFLAGS}

of-img-fullwidth: src/onefile/img_fullwidth.c
	${CC} ${CFLAGS} -x c src/onefile/img_fullwidth.c -o build/of-img_fullwidth.elf ${LDFLAGS}

benchmark: src/lib/pixel.c src/lib/graphics.c src/benchmark.c
	${CC} ${CFLAGS} -x c src/benchmark.c src/lib/graphics.c src/lib/pixel.c -o build/benchmark.elf ${LDFLAGS}
# ************************ BINARIES ************************

graphics.o: src/lib/graphics.c src/lib/graphics.h
	${CC} ${CFLAGS} -c src/lib/graphics.c -o build/bin/graphics.o

pixel.o: src/lib/pixel.c src/lib/pixel.h
	${CC} ${CFLAGS} -c src/lib/pixel.c -o build/bin/pixel.o

main.o: src/main.c src/lib/graphics.h src/lib/pixel.h
	${CC} ${CFLAGS} -c src/main.c -o build/bin/main.o
	
# ************************ LIBRARY ************************

lib: graphics.o pixel.o
	${AR} rcs build/bin/libgfx.a build/bin/graphics.o build/bin/pixel.o

clean:
	rm -f build/bin/*.elf build/bin/*.o build/bin/*.a build/*.elf
