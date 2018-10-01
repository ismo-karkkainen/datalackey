#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Usage: $(basename $0) datalackey-executable"
    exit 100
fi
B=$(basename $0 .sh)
DL=$1
OUT="${B}_out.txt"
EXP="${B}_expected.txt"

echo '{"label":1234,"label2":13}{"label":1234,"label2:13"}' |
$DL -m -i stdin JSON -o stdout JSON > $OUT

cat > $EXP <<EOF
[null,"data","stored","label","label2"]
[null,"error","format"]
EOF

diff -bq $OUT $EXP && rm -f $OUT $EXP
