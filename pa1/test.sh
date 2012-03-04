#!/bin/sh

for dim in 0 2 3 4
do
#    for n in 16 32 64 128 256 512 1024 2048 4096
    for n in 8192
    do
        #echo "running ./randmst 0 $n 5 $dim | cut -d\" \" -f8"
        ./randmst 0 $n 5 $dim
    done
    echo
    echo
done

