#!/bin/bash

TARGET=${1}

case "$TARGET" in
    miyoo|rg350|retrofw)
        docker run --rm -v "$(pwd)":/workspace simplemenu-builder /bin/sh -c "make clean && build-sm $TARGET"
        ;;
    *)
        echo "Error: Invalid target '$TARGET'. Valid targets are: miyoo, rg350, retrofw"
        exit 1
        ;;
esac
