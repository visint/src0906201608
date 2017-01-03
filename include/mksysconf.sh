#!/bin/sh
arm-linux-gcc -o sysconf  sysconf.c -I../include -L=./ -L/home/visint/src/arm/lib -lvispace -liconv -lsqlite3

