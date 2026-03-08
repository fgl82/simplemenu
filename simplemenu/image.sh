#!/bin/bash
# 1. Check for required toolchain files in current dir
if [[ ! -f "opendingux-gcw0-toolchain.2022-09-22.tar.xz" ]] || [[ ! -f "mipsel-RetroFW-linux-uclibc_sdk-buildroot.tar.gz" ]]; then
    echo "Error: Toolchain tarballs not found in current directory."
    echo "Please ensure OpenDingux and RetroFW files are downloaded here."
    exit 1
fi

# 2. Build the Docker image
echo "Building simplemenu-builder image..."
DOCKER_BUILDKIT=1 docker build -t simplemenu-builder .

echo "Done! Image 'simplemenu-builder' is ready."
