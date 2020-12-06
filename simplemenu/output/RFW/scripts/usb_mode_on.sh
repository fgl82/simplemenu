#!/bin/sh
umount /dev/mmcblk1p1
modprobe g_file_storage file=/dev/mmcblk1p1,/dev/mmcblk0p3 stall=0 removable=1
