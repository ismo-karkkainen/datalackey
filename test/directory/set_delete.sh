#!/bin/sh

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
echo '[1,"storage-info"]'
echo '[2,"delete","missing","label",null,4]'
echo '[3,"delete","missing","label"]'
echo '[4,"storage-info"]'
) | $DL -d "$STORE" -i stdin JSON -o stdout JSON | oneline-keysort-json > $OUT

cat > $EXP <<EOF
[null,"data","stored","label",1]
[1,"storage-info","",{"label":{"JSON":4,"serial":1}}]
[1,"done",""]
[2,"error","not-string",2,"delete","missing","label",null]
[2,"done",""]
[null,"data","deleted","label",1]
[3,"delete","missing","missing"]
[3,"done",""]
[4,"storage-info","",{}]
[4,"done",""]
EOF

COUT="$(pwd)/td/.datalackey/catalog"
CEXP="${B}_cat_expected.txt"
cat > $CEXP <<EOF
{}
EOF

test 1 -eq $(ls $STORE/.datalackey/ | wc -w) &&
compare-output $COUT $CEXP &&
compare-output $OUT $EXP &&
rm -rf $OUT $EXP $COUT $CEXP "$STORE"
