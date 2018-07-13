#!/bin/bash
cd $1
num=`find . -size +1048576c | wc -l`
sum=`find . -name "*" -exec wc -c {} \;|awk '{sum += $1};END {print sum}' |
awk 'END {print $1}'`
echo "The sum is "$sum" and There are "$num" files bigger than 1MB"

