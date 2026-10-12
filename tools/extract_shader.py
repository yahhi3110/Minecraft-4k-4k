# mc4k.cpp からシェーダー部分を取り出して標準出力へ書く
import re, sys
t = open(sys.argv[1] if len(sys.argv) > 1 else "mc4k.cpp").read()
sys.stdout.write(re.search(r'R"\((.*?)\)";', t, re.S).group(1))
