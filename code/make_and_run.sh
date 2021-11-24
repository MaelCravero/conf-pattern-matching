#!/bin/sh

for arg; do
    make $arg && ./$arg
done
