#!/bin/bash
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
DEVICE="GW2AR-LV18QN88C8/I7"
FAMILY="GW2A-18C"
CST_FILE="gw2ar-reflist.cst"
TOP_MODULE="top"
BOARD="tangnano20k"
MODE="${1:-flash}"

cd "$SCRIPT_DIR"

echo "=== Tang Nano 20K (GW2AR-LV18) Build + Flash ==="
echo "Device: $DEVICE"
echo "CST: $CST_FILE"
echo "Top: $TOP_MODULE"
echo "Mode: $MODE"
echo ""

if [ ! -f "${TOP_MODULE}.v" ]; then
    echo "ERROR: ${TOP_MODULE}.v not found in $SCRIPT_DIR"
    exit 1
fi

if [ ! -f "${CST_FILE}" ]; then
    echo "ERROR: ${CST_FILE} not found in $SCRIPT_DIR"
    exit 1
fi

echo "[1/5] Synthesizing with Yosys..."
yosys -p "read_verilog ${TOP_MODULE}.v; synth_gowin -top ${TOP_MODULE} -json ${TOP_MODULE}.json"

echo "[2/5] Place & Route with nextpnr-himbaechel-gowin..."
nextpnr-himbaechel-gowin \
    --json ${TOP_MODULE}.json \
    --write ${TOP_MODULE}_pnr.json \
    --device "${DEVICE}" \
    --vopt family="${FAMILY}" \
    --vopt cst="${CST_FILE}"

echo "[3/5] Packing bitstream..."
gowin_pack \
    --device "${FAMILY}" \
    --output ${TOP_MODULE}.fs \
    ${TOP_MODULE}_pnr.json

echo "[4/5] Build complete: ${TOP_MODULE}.fs"

case "$MODE" in
    sram)
        echo "[5/5] Programming to SRAM (volatile)..."
        openFPGALoader -m -b ${BOARD} ${TOP_MODULE}.fs
        ;;
    flash)
        echo "[5/5] Programming to Flash (persistent)..."
        openFPGALoader -f -b ${BOARD} ${TOP_MODULE}.fs
        ;;
    build)
        echo "[5/5] Build-only mode; skipping flash."
        ;;
    *)
        echo "Usage: $0 [sram|flash|build]"
        echo "  sram  - load bitstream to SRAM (lost on power-off)"
        echo "  flash - write bitstream to SPI flash (persistent)"
        echo "  build - build only, do not flash (default)"
        exit 1
        ;;
esac
