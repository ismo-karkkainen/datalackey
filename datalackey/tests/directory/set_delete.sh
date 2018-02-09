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

echo '{"label":1234}[1,"list"][2,"delete","missing","label",null,4][3,"list"]' |
$DL -d "$STORE" -i stdin JSON -o stdout JSON > $OUT

cat > $EXP <<EOF
[null,"stored","label"]
[1,{"label":{"JSON":4}}]
[2,"invalid",null,4]
[2,"missing","missing"]
[2,"deleted","label"]
[3,{}]
EOF

COUT="$(pwd)/td/.datalackey/catalog"
CEXP="${B}_cat_expected.txt"
cat > $CEXP <<EOF
{}
EOF

test 1 -eq $(ls $STORE/.datalackey/ | wc -w) &&
diff -bq $COUT $CEXP &&
diff -bq $OUT $EXP &&
rm -rf $OUT $EXP $COUT $CEXP "$STORE"
