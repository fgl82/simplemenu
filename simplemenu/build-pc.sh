#!/bin/bash

PORTABLE_DIR="simplemenu-portable"

# 1. Compile inside Docker
echo "Compiling SimpleMenu for PC..."
docker run --rm -v $(pwd):/workspace simplemenu-builder /bin/sh -c "make clean && build-sm pc"

if [ ! -f "simplemenu" ]; then
    echo "ERROR: Compilation failed. Binary not found."
    exit 1
fi

# 2. Setup Portable Folder Structure
echo "Setting up portable bundle..."
mkdir -p "$PORTABLE_DIR/libs"

# 3. Copy Binary and Resources
cp simplemenu "$PORTABLE_DIR/"

# 4. Harvest Libraries (Excluding core Arch/System libs)
echo "Harvesting libraries..."
docker run --rm -v $(pwd):/workspace simplemenu-builder bash -c "
ldd ./simplemenu | grep '=> /' | \
grep -vE 'libc\.so|libpthread\.so|libm\.so|libdl\.so|ld-linux|libgcc|librt\.so|libstdc\+\+|libX|libxcb|libXau|libXdmcp|libasound|libpulse' | \
awk '{print \$3}' | xargs -I '{}' cp -n '{}' /workspace/$PORTABLE_DIR/libs/"

# 5. Create the AppRun Launcher
# The HOME="\$HERE" trick ensures SimpleMenu creates its config folder 
# INSIDE the portable folder rather than your Arch $HOME.
cat << 'EOF' > "$PORTABLE_DIR/AppRun"
#!/bin/bash
HERE="$(dirname "$(readlink -f "${0}")")"
export LD_LIBRARY_PATH="$HERE/libs:$LD_LIBRARY_PATH"
export HOME="$HERE"
cd "$HERE"
# Create config directory if SimpleMenu expects it
mkdir -p .simplemenu
exec ./simplemenu "$@"
EOF

chmod +x "$PORTABLE_DIR/AppRun"

echo "------------------------------------------------"
echo "Success! SimpleMenu is ready in ./$PORTABLE_DIR"
echo "Launch it with: ./$PORTABLE_DIR/AppRun"
echo "------------------------------------------------"
