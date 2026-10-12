#!/bin/sh
# mc4k.cpp が4096バイト以下か確認する
n=$(wc -c < "$(dirname "$0")/../mc4k.cpp")
echo "mc4k.cpp: $n / 4096 bytes"
[ "$n" -le 4096 ]
