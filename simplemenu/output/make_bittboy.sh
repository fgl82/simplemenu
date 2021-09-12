#!/bin/sh
cd ..
make clean
make PLATFORM=BITTBOY
cp output/simplemenu /media/bittboy/main/apps/simplemenu/
cd ..
cd ..
cd invoker
cd invoker
make clean
make PLATFORM=BITTBOY
cp invoker.dge /media/bittboy/main/apps/simplemenu/
umount /media/bittboy/boot
umount /media/bittboy/main
umount /media/bittboy/rootfs

