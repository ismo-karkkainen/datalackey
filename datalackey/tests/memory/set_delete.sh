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
echo '{"label":1234}'
echo '[1,"storage-info"]'
echo '[2,"delete","missing","label",null,4]'
echo '[3,"delete","missing","label",4]'
echo '[4,"delete","missing","label"]'
echo '[5,"list"]'
) | $DL -m -i stdin JSON -o stdout JSON > $OUT

cat > $EXP <<EOF
[null,"data","stored","label"]
[1,"storage-info","",{"label":{"JSON":4}}]
[2,"error","not-string",2,"delete","missing","label",null]
[3,"error","not-string",3,"delete","missing","label",4]
[4,"delete","missing","missing"]
[4,"delete","deleted","label"]
[5,"list",""]
EOF

compare-output $OUT $EXP && rm -f $OUT $EXP
