#! /bin/bash
gcc -c checksums.c
ar rcs checksums.a checksums.o
