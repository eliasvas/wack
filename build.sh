#!/usr/bin/env bash
set -e

ENGINE_DIR="./prototype"
GAME_DIR="./game"
OUTPUT_DIR="./build"

"$ENGINE_DIR/build.sh" gd="$GAME_DIR" od="$OUTPUT_DIR"
