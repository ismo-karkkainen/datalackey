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
echo '[1,"storage-info"]'
echo '[2,"rename","label"]'
echo '[3,"rename","label","label2"]'
echo '[4,"storage-info"]'
echo '[5,"rename","label2","label3"]'
echo '[6,"storage-info"]'
) | $DL -m -i stdin JSON -o stdout JSON |
sed 's/"running",.*]$/"running",pid]/' > $OUT

cat > $EXP <<EOF
[null,"data","stored","label","label2"]
[1,"storage-info","",{"label":{"JSON":3},"label2":{"JSON":1}}]
[2,"rename","error","argument","pairless"]
[3,"rename","renamed","label","label2"]
[4,"storage-info","",{"label2":{"JSON":3}}]
[5,"rename","renamed","label2","label3"]
[6,"storage-info","",{"label3":{"JSON":3}}]
EOF

compare-output $OUT $EXP && rm -f $OUT $EXP
