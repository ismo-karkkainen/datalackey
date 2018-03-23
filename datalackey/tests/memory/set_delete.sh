#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Usage: $(basename $0) datalackey-executable"
    exit 100
fi
B=$(basename $0 .sh)
DL=$1
OUT="${B}_out.txt"
EXP="${B}_expected.txt"

echo '{"label":1234}[1,"storage-info"][2,"delete","missing","label",null,4][3,"list"]' |
$DL -m -i stdin JSON -o stdout JSON > $OUT

cat > $EXP <<EOF
[null,"data","stored","label"]
[1,"storage-info","",{"label":{"JSON":4}}]
[2,"delete","invalid",null,4]
[2,"delete","missing","missing"]
[2,"delete","deleted","label"]
[3,"list",""]
EOF

diff -bq $OUT $EXP && rm -f $OUT $EXP
