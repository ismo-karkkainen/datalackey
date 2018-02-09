#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Usage: $(basename $0) datalackey-executable"
    exit 100
fi
B=$(basename $0 .sh)
DL=$1
OUT="${B}_out.txt"
EXP="${B}_expected.txt"

(
echo '{"label":123,"label2":4}'
echo '[1,"list"]'
echo '[2,"rename","label"]'
echo '[3,"rename","label","label2"]'
echo '[4,"list"]'
echo '[5,"rename","label2","label3"]'
echo '[6,"list"]'
) | $DL -m -i stdin JSON -o stdout JSON |
sed 's/"running",.*]$/"running",pid]/' > $OUT

cat > $EXP <<EOF
[null,"stored","label","label2"]
[1,{"label":{"JSON":3},"label2":{"JSON":1}}]
[2,"error","argument","pairless"]
[3,"renamed","label","label2"]
[4,{"label2":{"JSON":3}}]
[5,"renamed","label2","label3"]
[6,{"label3":{"JSON":3}}]
EOF

diff -bq $OUT $EXP && rm -f $OUT $EXP
