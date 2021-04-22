#!/usr/bin/env bash
make
./a.out
make clean
make debug
./debug.out>t2
diff temp t2
make clean
rm t2