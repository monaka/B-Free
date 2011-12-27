#!/bin/sh
./mksfs /dev/fd0 2880 512 12
./statfs /dev/fd0 mkdir /dev
./statfs /dev/fd0 mknod /dev/console	0x00000000
./statfs /dev/fd0 mknod /dev/win1	0x00000001
./statfs /dev/fd0 mknod /dev/win2	0x00000002
./statfs /dev/fd0 mknod /dev/keyboard	0x00010000
./statfs /dev/fd0 mknod /dev/fd		0x80000000
./statfs /dev/fd0 mknod /dev/ide0	0x80010000
./statfs /dev/fd0 mknod /dev/ide1	0x80010001
./statfs /dev/fd0 mknod /dev/ide2	0x80010002
./statfs /dev/fd0 mknod /dev/ide3	0x80010003
./statfs /dev/fd0 mknod /dev/ide4	0x80010004
echo dir /
./statfs /dev/fd0 dir /
echo dir /dev
./statfs /dev/fd0 dir /dev
