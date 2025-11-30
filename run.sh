#!/usr/bin/env bash
set -e

BUILD_DIR="./build"

if [ ! -d "$BUILD_DIR" ]; then
    cmake --preset=default
fi

if [ -z "$1" ]; then
    echo "Usage: $0 <target>"
    echo "Usage: $0 test <target>"
    exit 1
fi

TARGET="$1"

if [ "$TARGET" = "test" ]; then
  if [ -z "$2" ]; then
    ninja -C "$BUILD_DIR" testall
    exit $?
  fi
  TARGET="${2}_tests"
  TARGET=$(echo "$TARGET" | tr '[:upper:]' '[:lower:]')
fi

ninja -C "$BUILD_DIR" "$TARGET" && echo "Compilation success"

"$BUILD_DIR/bin/"$TARGET"

