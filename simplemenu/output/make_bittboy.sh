#!/bin/sh
cd ..
make clean
make PLATFORM=BITTBOY
cp output/simplemenu /media/bittboy/main/apps/simplemenu
umount /media/bittboy/boot
umount /media/bittboy/main
umount /media/bittboy/rootfs
cd output

