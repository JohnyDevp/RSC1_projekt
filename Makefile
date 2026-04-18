CC=/usr/lib/ripes/lib/riscv64-unknown-elf-gcc-8.3.0-2020.04.1-x86_64-linux-ubuntu14/bin/riscv64-unknown-elf-gcc
AR=/usr/lib/ripes/lib/riscv64-unknown-elf-gcc-8.3.0-2020.04.1-x86_64-linux-ubuntu14/bin/riscv64-unknown-elf-ar
CFLAGS=-march=rv32imc -mabi=ilp32 -O0 -g
LDFLAGS=-static -lm
TEST_CASES=1 2 3 4 5 6 7 8 9 10
TEST_BINS=$(addprefix build/tests/test_,$(addsuffix .elf,$(TEST_CASES)))
RIPES=/usr/lib/ripes/bin/Ripes
RIPES_PROC=RV32_5S
RIPES_TIMEOUT=50000
RIPES_ISAEXTS=M,C

# ************************ EXECUTABLES ************************

# make using library
all-lib: lib main.o
	${CC} ${CFLAGS} build/bins/main.o -o build/main.elf -Lbuild/bins -lgfx ${LDFLAGS}

# make without library, just binaries
all: pixel.o graphics.o main.o
	${CC} ${CFLAGS} build/bins/main.o build/bins/graphics.o build/bins/pixel.o -o build/main.elf ${LDFLAGS}

my-test: tests/my_test.c src/lib/pixel.c src/lib/pixel.h src/lib/graphics.c src/lib/graphics.h
	${CC} ${CFLAGS} tests/my_test.c src/lib/pixel.c src/lib/graphics.c -o build/tests/my_test.elf ${LDFLAGS}

test: lib ${TEST_BINS}

run-test-%: build/tests/test_%.elf
	${RIPES} --mode cli --src build/tests/test_$*.elf -t elf --proc ${RIPES_PROC} --isaexts ${RIPES_ISAEXTS} --timeout ${RIPES_TIMEOUT} --all

run-tests: test
	for t in ${TEST_CASES}; do \
		${RIPES} --mode cli --src build/tests/test_$$t.elf -t elf --proc ${RIPES_PROC} --isaexts ${RIPES_ISAEXTS} --timeout ${RIPES_TIMEOUT} --all || exit $$?; \
	done

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

build/tests/test_%.elf: tests/tests.c lib
	${CC} ${CFLAGS} -DTEST_CASE=$* tests/tests.c -o $@ -Lbuild/bins -lgfx ${LDFLAGS}

clean:
	rm -f build/bins/*.elf build/bins/*.o build/bins/*.a build/*.elf
