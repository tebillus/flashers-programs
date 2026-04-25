#!/bin/bash
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

if [ -z "$1" ]; then
  echo "Usage: $0 <target>"
  echo ""
  echo "Builds <target>.c (plus startup.c and system_stm32f4xx.c) into <target>.bin"
  echo ""
  echo "Examples:"
  echo "  $0 blinky"
  exit 1
fi

TARGET="$1"
SRC="${TARGET}.c"

if [ ! -f "$SRC" ]; then
  echo "ERROR: source file '$SRC' not found"
  exit 1
fi

# --- Toolchain ---
TOOLCHAIN="arm-none-eabi-"
CC="${TOOLCHAIN}gcc"
OBJCOPY="${TOOLCHAIN}objcopy"
SIZE="${TOOLCHAIN}size"

if ! command -v "$CC" >/dev/null 2>&1; then
  echo "ERROR: $CC not found in PATH"
  exit 1
fi

# --- Flags (STM32F411) ---
CFLAGS="-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard"
CFLAGS="$CFLAGS -O2 -Wall -g"
CFLAGS="$CFLAGS -ffunction-sections -fdata-sections"

LDFLAGS="-T linker_script.ld -nostartfiles -Wl,--gc-sections"

BUILD_DIR="build"
mkdir -p "$BUILD_DIR"

# --- Compile ---
echo "=== Building $TARGET ==="

echo "  CC    startup.c"
$CC $CFLAGS -c startup.c -o "$BUILD_DIR/startup.o"

echo "  CC    system_stm32f4xx.c"
$CC $CFLAGS -c system_stm32f4xx.c -o "$BUILD_DIR/system_stm32f4xx.o"

echo "  CC    $SRC"
$CC $CFLAGS -c "$SRC" -o "$BUILD_DIR/${TARGET}.o"

echo "  LD    $TARGET.elf"
$CC $CFLAGS $LDFLAGS \
  "$BUILD_DIR/startup.o" \
  "$BUILD_DIR/system_stm32f4xx.o" \
  "$BUILD_DIR/${TARGET}.o" \
  -o "$BUILD_DIR/$TARGET.elf"

echo "  BIN   $TARGET.bin"
$OBJCOPY -O binary "$BUILD_DIR/$TARGET.elf" "$TARGET.bin"

echo "  SIZE"
$SIZE "$BUILD_DIR/$TARGET.elf"

echo ""
echo "=== Build complete ==="
echo "  Binary: $TARGET.bin"
echo "  ELF:    $BUILD_DIR/$TARGET.elf"
echo "  Objects in: $BUILD_DIR/"
echo ""
echo "To archive this build, rename the folder:  mv $BUILD_DIR ${BUILD_DIR}xx"
