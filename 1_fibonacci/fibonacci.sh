#!/bin/bash
a=1
b=1
c=$(($a+$b))
name=0
size=0
file=0
if [ "$1" == "--clean" ]; then
    find . -name "*.bin" -exec rm -f {} \;
else
    for((i=1;i<=45;i++))
        do
        c=$(($a+$b))
        name=`printf '%04d' $i`
        size=`printf '%d' $a`
        file=$name.bin
        dd if=/dev/zero of=$file bs=1b count=$size
        a=$b
        b=$c
        done
fi

