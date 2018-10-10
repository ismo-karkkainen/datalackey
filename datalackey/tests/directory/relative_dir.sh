#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Usage: $(basename $0) datalackey-executable"
    exit 100
fi
B=$(basename $0 .sh)
DL=$1
OUT="${B}_out.txt"
EXP="${B}_expected.txt"
STORE="td"

rm -rf "$STORE"
mkdir "$STORE"

echo | $DL -d "$STORE" -i stdin JSON -o stdout JSON > $OUT

cat > $EXP <<EOF
EOF

test 1 -eq $(ls $STORE/.datalackey/ | wc -w) &&
test -f $STORE/.datalackey/catalog &&
compare-output $OUT $EXP &&
rm -rf $OUT $EXP "$STORE"
