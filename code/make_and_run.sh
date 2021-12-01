#!/bin/sh

for arg; do
    make -B $arg && ./$arg
done
