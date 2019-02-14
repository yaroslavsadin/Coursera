#!/bin/sh

g++ -g -O3 $1 -o $1.elf
./$1.elf