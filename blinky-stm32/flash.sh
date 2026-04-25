#!/bin/bash
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

FLASH_ADDR="0x08000000"

# --- Toolchain ---
TOOLCHAIN="arm-none-eabi-"
CC="${TOOLCHAIN}gcc"
OBJCOPY="${TOOLCHAIN}objcopy"
SIZE="${TOOLCHAIN}size"

CFLAGS="-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard"
CFLAGS="$CFLAGS -O2 -Wall -g"
CFLAGS="$CFLAGS -ffunction-sections -fdata-sections"

LDFLAGS="-T linker_script.ld -nostartfiles -Wl,--gc-sections"

BUILD_DIR="build"

# --- Build Logic ---
build_target() {
    local target="$1"
    local src="${target}.c"

    if [ ! -f "$src" ]; then
        echo "ERROR: source file '$src' not found"
        exit 1
    fi

    if ! command -v "$CC" >/dev/null 2>&1; then
        echo "ERROR: $CC not found in PATH"
        exit 1
    fi

    mkdir -p "$BUILD_DIR"

    echo "=== Building $target ==="

    echo "  CC    startup.c"
    $CC $CFLAGS -c startup.c -o "$BUILD_DIR/startup.o"

    echo "  CC    system_stm32f4xx.c"
    $CC $CFLAGS -c system_stm32f4xx.c -o "$BUILD_DIR/system_stm32f4xx.o"

    echo "  CC    $src"
    $CC $CFLAGS -c "$src" -o "$BUILD_DIR/${target}.o"

    echo "  LD    $target.elf"
    $CC $CFLAGS $LDFLAGS \
        "$BUILD_DIR/startup.o" \
        "$BUILD_DIR/system_stm32f4xx.o" \
        "$BUILD_DIR/${target}.o" \
        -o "$BUILD_DIR/$target.elf"

    echo "  BIN   $target.bin"
    $OBJCOPY -O binary "$BUILD_DIR/$target.elf" "$target.bin"

    echo "  SIZE"
    $SIZE "$BUILD_DIR/$target.elf"

    echo ""
    echo "=== Build complete ==="
    echo "  Binary: $target.bin"
    echo "  ELF:    $BUILD_DIR/$target.elf"
}

# --- Upload Logic ---
upload_binary() {
    local bin_file="$1"

    if [ -z "$bin_file" ]; then
        echo "Usage: $0 upload <binary.bin>"
        echo ""
        echo "Available binaries:"
        ls -1 *.bin 2>/dev/null | sed 's/^/  /' || echo "  (none found)"
        exit 1
    fi

    if [ ! -f "$bin_file" ]; then
        echo "ERROR: $bin_file not found"
        exit 1
    fi

    if ! command -v dfu-util >/dev/null 2>&1; then
        echo "ERROR: dfu-util not found in PATH"
        exit 1
    fi

    echo "=== Flash via dfu-util ==="
    echo ""
    echo "Binary:  $bin_file"
    echo "Address: $FLASH_ADDR"
    echo ""

    if ! dfu-util -l 2>/dev/null | grep -q "Found DFU"; then
        echo "No DFU device found."
        echo "Make sure the board is in DFU mode:"
        echo "  1. Hold BOOT0 (or bridge BOOT0 pin to 3.3V)"
        echo "  2. Press and release RESET"
        echo "  3. Release BOOT0"
        exit 1
    fi

    echo "DFU device detected, flashing..."
    dfu-util -a 0 -s ${FLASH_ADDR}:leave -D "$bin_file"

    echo ""
    echo "Flash complete. Board will reset and run your firmware."
}

# --- Main ---
if [ -z "${1:-}" ]; then
    echo "Usage: $0 <target>"
    echo "       $0 build <target>"
    echo "       $0 upload <binary.bin>"
    echo ""
    echo "Examples:"
    echo "  $0 blinky          # build then flash blinky.c"
    echo "  $0 build blinky    # build only"
    echo "  $0 upload blinky.bin # flash only"
    exit 1
fi

case "$1" in
    build)
        if [ -z "${2:-}" ]; then
            echo "Usage: $0 build <target>"
            exit 1
        fi
        build_target "$2"
        ;;
    upload)
        upload_binary "${2:-}"
        ;;
    *)
        # Default: build then flash
        TARGET="$1"
        build_target "$TARGET"
        echo ""
        upload_binary "$TARGET.bin"
        ;;
esac
