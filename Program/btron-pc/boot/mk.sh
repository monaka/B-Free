make clean
make hdd
cd tool
make
./writeboot -w -v /dev/hda 3 ../bootimage_hd
dd if=../bootimage_hd of=/dev/hda3

