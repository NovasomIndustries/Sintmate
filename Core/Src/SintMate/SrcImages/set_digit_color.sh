#!/bin/sh
[ -z $1 ] && return 1
color=$1
for i in 0 1 2 3 4 5 6 7 8 9; do
	rm -f digit$i.bmp
	ln -s $color$i.bmp digit$i.bmp
done
