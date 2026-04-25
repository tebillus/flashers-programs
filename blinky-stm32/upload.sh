#!/bin/bash
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
: "${FLASH_ADDR:=0x08000000}"

cd "$SCRIPT_DIR"

BIN_FILE=""

if [ -n "$1" ]; then
    BIN_FILE="$1"
else
    echo "Usage: $0 <binary.bin>"
    echo ""
    echo "Available binaries in project root:"
    ls -1 *.bin 2>/dev/null | sed 's/^/  /' || echo "  (none found)"
    exit 1
fi

if [ ! -f "$BIN_FILE" ]; then
    echo "ERROR: $BIN_FILE not found"
    exit 1
fi

if ! command -v dfu-util >/dev/null 2>&1; then
    echo "ERROR: dfu-util not found in PATH"
    exit 1
fi

echo "=== Flash via dfu-util ==="
echo ""
echo "Binary:  $BIN_FILE"
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
dfu-util -a 0 -s ${FLASH_ADDR}:leave -D "$BIN_FILE"

echo ""
echo "Flash complete. Board will reset and run your firmware."
