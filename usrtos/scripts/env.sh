#!/bin/sh
sh usrtos/scripts/post-commit

mkdir work
mkdir -p /tmp/usrtos
rm /tmp/usrtos/*

python3 usrtos/python/usrtos/layout.py usrtos/memory.json

make clean

make all
make workers
make examples

touch /tmp/usrtos/test.result

