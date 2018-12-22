#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Usage: $(basename $0) datalackey-executable"
    exit 100
fi
B=$(basename $0 .sh)
DL=$1
OUT="${B}_out.txt"
EXP="${B}_expected.txt"
STORE="$(pwd)/td"

rm -rf "$STORE"
mkdir "$STORE"

(
echo '{"label":1234}'
nap
echo '[1,"storage-info"]'
) | $DL -d "$STORE" -i stdin JSON -o stdout JSON | oneline-keysort-json > $OUT

echo '[2,"storage-info"]' |
$DL -d "$STORE" -i stdin JSON -o stdout JSON | oneline-keysort-json >> $OUT

cat > $EXP <<EOF
[null,"data","stored","label",1]
[1,"storage-info","",{"label":{"JSON":4,"serial":1}}]
[1,"done",""]
[null,"data","stored","label",1]
[2,"storage-info","",{"label":{"JSON":4,"serial":1}}]
[2,"done",""]
EOF

COUT="$(pwd)/td/.datalackey/catalog"
CEXP="${B}_cat_expected.txt"
cat > $CEXP <<EOF
{"label":["JSON",1]}
EOF

test 2 -eq $(ls $STORE/.datalackey/ | wc -w) &&
test -f $STORE/.datalackey/10 &&
compare-output $COUT $CEXP &&
compare-output $OUT $EXP &&
rm -rf $OUT $EXP $COUT $CEXP "$STORE"
