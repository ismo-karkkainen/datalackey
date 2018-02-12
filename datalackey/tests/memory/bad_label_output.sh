#!/bin/bash

if [ $# -ne 2 ]; then
    echo "Usage: $(basename $0) bad-label datalackey-executable"
    exit 100
fi
B=$(basename $0 .sh)
L=$1
DL=$2
OUT="${B}_out.txt"
EXP="${B}_expected.txt"

(
echo '{ $L: "value" }'
printf '\0'
echo '[2,"storage-info"]'
) | $DL -m -i stdin JSON -o stdout JSON > $OUT

cat > $EXP <<EOF
[null,"error","format"]
[null,"note","reset"]
[2,{}]
EOF

diff -bq $OUT $EXP && rm -f $OUT $EXP
