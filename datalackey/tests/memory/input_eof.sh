#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Usage: $(basename $0) datalackey-executable"
    exit 100
fi
B=$(basename $0 .sh)
DL=$1
OUT="${B}_out.txt"
EXP="${B}_expected.txt"

cat > _script.sh << EOF
#!/usr/bin/python

from __future__ import print_function
import json
import sys
import os

raw = sys.stdin.read()
try:
    data = json.loads(raw)
except ValueError, e:
    print(str(e), file=sys.stderr)
    print(raw, file=sys.stderr)
    sys.exit(1)
print(json.dumps(dict(os.environ)), file=sys.stderr)
print(json.dumps(sys.argv), file=sys.stderr)
dada = {}
for d in data:
    dada[d + "-out"] = data[d]
print(json.dumps(dada), file=sys.stdout)
sys.exit(0)
EOF
chmod a+x _script.sh

(
echo '{"label":123}'
echo '[1,"run","channel","in","JSON","stdin","channel","out","JSON","stdout","input","label","foo","output-prefix","fed-","end-feed","program","./_script.sh"]'
) | $DL -m -i stdin JSON -o stdout JSON |
sed 's/"running",.*]$/"running",pid]/' > $OUT

cat > $EXP <<EOF
[null,"data","stored","label"]
[1,"run","running",pid]
[1,"run","exit",0]
[1,"data","stored","fed-foo-out"]
[1,"run","finished"]
EOF

diff -bq $OUT $EXP && rm -f $OUT $EXP _script.sh
