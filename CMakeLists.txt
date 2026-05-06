cmake_minimum_required(VERSION 3.16)

project(RSC1 NONE)

# Use RISCV toolchain from user's Downloads directory (expand HOME)
set(CC $ENV{HOME}/Downloads/riscv64-unknown-elf-gcc-8.3.0-2020.04.1-x86_64-linux-centos6/bin/riscv64-unknown-elf-gcc)
set(AR $ENV{HOME}/Downloads/riscv64-unknown-elf-gcc-8.3.0-2020.04.1-x86_64-linux-centos6/bin/riscv64-unknown-elf-ar)

set(CFLAGS -march=rv32im -mabi=ilp32 -O0 -g)
set(LDFLAGS -static -lm)

set(BUILD_DIR ${CMAKE_SOURCE_DIR}/build)
set(BIN_DIR ${BUILD_DIR}/bin)

add_custom_target(prepare
    COMMAND ${CMAKE_COMMAND} -E make_directory ${BIN_DIR}
)

# Build library and main linked against it
add_custom_target(lib
    COMMAND ${CC} ${CFLAGS} -c ${CMAKE_SOURCE_DIR}/src/lib/graphics.c -o ${BIN_DIR}/graphics.o
    COMMAND ${CC} ${CFLAGS} -c ${CMAKE_SOURCE_DIR}/src/lib/pixel.c -o ${BIN_DIR}/pixel.o
    COMMAND ${AR} rcs ${BIN_DIR}/libgfx.a ${BIN_DIR}/graphics.o ${BIN_DIR}/pixel.o
    DEPENDS prepare
)

add_custom_target(main-lib
    COMMAND ${CC} ${CFLAGS} -c ${CMAKE_SOURCE_DIR}/src/main.c -o ${BIN_DIR}/main.o
    COMMAND ${CC} ${CFLAGS} ${BIN_DIR}/main.o -o ${BUILD_DIR}/main.elf -L${BIN_DIR} -lgfx ${LDFLAGS}
    DEPENDS lib prepare
)

# Build single-file objects and link (no library)
add_custom_target(main
    COMMAND ${CC} ${CFLAGS} -c ${CMAKE_SOURCE_DIR}/src/lib/graphics.c -o ${BIN_DIR}/graphics.o
    COMMAND ${CC} ${CFLAGS} -c ${CMAKE_SOURCE_DIR}/src/lib/pixel.c -o ${BIN_DIR}/pixel.o
    COMMAND ${CC} ${CFLAGS} -c ${CMAKE_SOURCE_DIR}/src/main.c -o ${BIN_DIR}/main.o
    COMMAND ${CC} ${CFLAGS} ${BIN_DIR}/main.o ${BIN_DIR}/graphics.o ${BIN_DIR}/pixel.o -o ${BUILD_DIR}/main.elf ${LDFLAGS}
    DEPENDS prepare
)

add_custom_target(of-main
    COMMAND ${CC} ${CFLAGS} -x c ${CMAKE_SOURCE_DIR}/src/onefile/main.c -o ${BUILD_DIR}/of-main.elf ${LDFLAGS}
    DEPENDS prepare
)

add_custom_target(of-benchmark
    COMMAND ${CC} ${CFLAGS} -x c ${CMAKE_SOURCE_DIR}/src/onefile/benchmark.c -o ${BUILD_DIR}/of-benchmark.elf ${LDFLAGS}
    DEPENDS prepare
)

add_custom_target(of-img
    COMMAND ${CC} ${CFLAGS} -x c ${CMAKE_SOURCE_DIR}/src/onefile/img.c -o ${BUILD_DIR}/of-img.elf ${LDFLAGS}
    DEPENDS prepare
)

add_custom_target(of-img-fullwidth
    COMMAND ${CC} ${CFLAGS} -x c ${CMAKE_SOURCE_DIR}/src/onefile/img_fullwidth.c -o ${BUILD_DIR}/of-img_fullwidth.elf ${LDFLAGS}
    DEPENDS prepare
)

add_custom_target(img
    COMMAND ${CC} ${CFLAGS} -x c
            ${CMAKE_SOURCE_DIR}/src/img.c
            ${CMAKE_SOURCE_DIR}/src/lib/graphics.c
            ${CMAKE_SOURCE_DIR}/src/lib/pixel.c
            -o ${BUILD_DIR}/img.elf
            ${LDFLAGS}
    DEPENDS prepare
)

add_custom_target(all_targets
    DEPENDS main main-lib of-main of-benchmark of-img of-img-fullwidth
)

add_custom_target(clean-build
    COMMAND ${CMAKE_COMMAND} -E rm -f
            ${BIN_DIR}/main.o
            ${BIN_DIR}/graphics.o
            ${BIN_DIR}/pixel.o
            ${BIN_DIR}/libgfx.a
            ${BUILD_DIR}/main.elf
            ${BUILD_DIR}/of-main.elf
            ${BUILD_DIR}/of-benchmark.elf
            ${BUILD_DIR}/of-img.elf
            ${BUILD_DIR}/of-img_fullwidth.elf
)
