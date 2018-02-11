#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Usage: $(basename $0) datalackey-executable"
    exit 100
fi
B=$(basename $0 .sh)
DL=$1
OUT="${B}_out.txt"
EXP="${B}_expected.txt"

echo | $DL -d invalid -i stdin JSON -o stdout JSON > $OUT
test $? -ne 10 && echo "Exit code != 10" && exit 1

cat > $EXP <<EOF
EOF

diff -bq $OUT $EXP &&
rm -rf $OUT $EXP "$STORE"
