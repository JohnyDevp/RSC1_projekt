# RSC1 Projekt

RISC-V C project with multiple build options:

- static library build
- direct object build
- one-file examples
- benchmark build

## Downloads

- Ripes (official GitHub):
    - https://github.com/mortbopet/Ripes
    - Releases: https://github.com/mortbopet/Ripes/releases
- SiFive RISC-V toolchain (requested link):
    - https://github.com/sifive/freedom-tools/releases/tag/v2020.04.0-Toolchain.Only

## Project Structure

- Makefile
- CMakeLists.txt
- src/
    - main.c
    - benchmark.c
    - lib/
    - onefile/

## Linux Setup

1. Install required tools:
    - CMake (optional if using only Makefile)
    - Make
    - GNU toolchain from the SiFive release link above
2. Extract/install the SiFive toolchain.
3. Find the compiler binaries:
    - riscv64-unknown-elf-gcc
    - riscv64-unknown-elf-ar
4. Update paths in build files if needed:
    - Makefile: CC and AR
    - CMakeLists.txt: CC and AR

### Build with Makefile

From project root:

```bash
make all-lib
make all
make of-main
make of-benchmark
make benchmark
```

Artifacts are generated in build/ and build/bin/.

### Build with CMake

From project root:

```bash
cmake -S . -B build-cmake
cmake --build build-cmake --target all-lib
cmake --build build-cmake --target all-bins
cmake --build build-cmake --target of-main
cmake --build build-cmake --target of-benchmark
cmake --build build-cmake --target benchmark
cmake --build build-cmake --target clean-build
```

## Windows Setup

Recommended options:

- Use WSL (Ubuntu) for the easiest Linux-like workflow.
- Or use MSYS2/MinGW shell with make and cmake installed.

1. Download Ripes from the GitHub releases page above.
2. Download the SiFive toolchain from:
    - https://github.com/sifive/freedom-tools/releases/tag/v2020.04.0-Toolchain.Only
3. Extract the toolchain.
4. Set correct paths for:
    - riscv64-unknown-elf-gcc
    - riscv64-unknown-elf-ar
5. Update CC and AR in:
    - Makefile
    - CMakeLists.txt

### Build on Windows with CMake (PowerShell)

```powershell
cmake -S . -B build-cmake
cmake --build build-cmake --target all-lib
```

### Build on Windows with Make

If using WSL/MSYS2 and make is available:

```bash
make all-lib
```

## Running in Ripes

1. Open Ripes.
2. Load one of the produced ELF files, for example:
    - build/main.elf
    - build/of-main.elf
    - build/benchmark.elf
3. Select a matching RV32IM machine configuration in Ripes if needed.

## Notes

- Current flags in project:
    - march: rv32im
    - abi: ilp32
- If your toolchain is in a different location, update CC and AR accordingly.
