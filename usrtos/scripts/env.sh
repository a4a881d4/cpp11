#!/bin/sh
sh usrtos/scripts/post-commit

mkdir -p /tmp/usrtos

python3 usrtos/python/usrtos/layout.py usrtos/memory.json

make clean

make all
make workers


