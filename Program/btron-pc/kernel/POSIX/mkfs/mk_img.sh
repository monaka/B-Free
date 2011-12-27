#!/bin/sh
# make posix fd image
# 

rm -f posix_fd.img
./mksfs posix_fd.img 2880 512 12
./statfs posix_fd.img mkdir /dev
./statfs posix_fd.img mknod /dev/console	0x00000000
./statfs posix_fd.img mknod /dev/win1	0x00000001
./statfs posix_fd.img mknod /dev/win2	0x00000002
./statfs posix_fd.img mknod /dev/keyboard	0x00010000
./statfs posix_fd.img mknod /dev/fd		0x80000000
./statfs posix_fd.img mknod /dev/ide0	0x80010000
./statfs posix_fd.img mknod /dev/ide1	0x80010001
./statfs posix_fd.img mknod /dev/ide2	0x80010002
./statfs posix_fd.img mknod /dev/ide3	0x80010003
./statfs posix_fd.img mknod /dev/ide4	0x80010004
echo dir /
./statfs posix_fd.img dir /
echo dir /dev
./statfs posix_fd.img dir /dev
(cd ../app;make)
(../app/inst_app.sh)
./statfs posix_fd.img mkdir /system
./statfs posix_fd.img write /system/btron386 ../../BTRON/make/btron
./statfs posix_fd.img dir /
./statfs posix_fd.img dir /system
