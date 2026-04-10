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
