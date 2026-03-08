FROM miyoocfw/toolchain-shared-musl:latest AS miyoo_source
FROM debian:bookworm-slim

RUN apt-get update && apt-get install -y \
    build-essential git make wget curl ca-certificates xz-utils unzip pkg-config \
    libsdl1.2-dev libsdl-image1.2-dev libsdl-ttf2.0-dev libsdl-mixer1.2-dev \
    libpng-dev libjpeg-dev zlib1g-dev libconfuse-dev \
    && apt-get clean && rm -rf /var/lib/apt/lists/*

WORKDIR /opt/toolchains
COPY --from=miyoo_source /opt/miyoo /opt/toolchains/miyoo

# Extract local toolchains
COPY opendingux-gcw0-toolchain.2022-09-22.tar.xz /tmp/
RUN mkdir rg350 && tar -xJf /tmp/opendingux-gcw0-toolchain.2022-09-22.tar.xz -C rg350 --strip-components=1 && rm /tmp/*.tar.xz

COPY mipsel-RetroFW-linux-uclibc_sdk-buildroot.tar.gz /tmp/
RUN mkdir retrofw && tar -xzf /tmp/mipsel-RetroFW-linux-uclibc_sdk-buildroot.tar.gz -C retrofw --strip-components=1 && rm /tmp/*.tar.gz

# Prep extra sources
RUN mkdir -p /opt/extra_src && cd /tmp && \
    curl -fSL https://github.com/pcercuei/libini/archive/master.tar.gz | tar -xz && \
    curl -fSL https://github.com/pcercuei/libopk/archive/master.tar.gz | tar -xz && \
    find . -name "*.h" -exec cp {} /opt/extra_src/ \; && \
    find . -name "*.c" -exec cp {} /opt/extra_src/ \;

RUN echo '#!/bin/bash\n\
case "$1" in\n\
  miyoo)   export PATH="/opt/toolchains/miyoo/bin:$PATH"; export CROSS_COMPILE=arm-linux-; export SYSROOT="/opt/toolchains/miyoo/arm-miyoo-linux-musleabi/sysroot" ;;\n\
  rg350)   export PATH="/opt/toolchains/rg350/bin:$PATH"; export CROSS_COMPILE=mipsel-linux-; export SYSROOT="/opt/toolchains/rg350/usr/mipsel-gcw0-linux-uclibc/sysroot" ;;\n\
  retrofw) export PATH="/opt/toolchains/retrofw/bin:$PATH"; export CROSS_COMPILE=mipsel-linux-; export SYSROOT="/opt/toolchains/retrofw/mipsel-RetroFW-linux-uclibc/sysroot" ;;\n\
  pc)      export CROSS_COMPILE="" ;;\n\
esac\n\
export CFLAGS="-I/opt/extra_src ${CFLAGS}"\n\
export EXTRA_SRCS="/opt/extra_src/libini.c /opt/extra_src/libopk.c /opt/extra_src/unsqfs.c"\n\
make TARGET=$1 EXTRA_SRCS="$EXTRA_SRCS"' > /usr/local/bin/build-sm && chmod +x /usr/local/bin/build-sm

WORKDIR /workspace
