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
sleep 1
echo '[1,"storage-info"]'
) | $DL -d "$STORE" -i stdin JSON -o stdout JSON > $OUT

echo '[2,"storage-info"]' |
$DL -d "$STORE" -i stdin JSON -o stdout JSON >> $OUT

cat > $EXP <<EOF
[null,"stored","label"]
[1,{"label":{"JSON":4}}]
[2,{"label":{"JSON":4}}]
EOF

COUT="$(pwd)/td/.datalackey/catalog"
CEXP="${B}_cat_expected.txt"
cat > $CEXP <<EOF
{"label":["JSON",1]}
EOF

test 2 -eq $(ls $STORE/.datalackey/ | wc -w) &&
test -f $STORE/.datalackey/10 &&
diff -bq $COUT $CEXP &&
diff -bq $OUT $EXP &&
rm -rf $OUT $EXP $COUT $CEXP "$STORE"
