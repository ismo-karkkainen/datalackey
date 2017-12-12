#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Usage: $(basename $0) datalackey-executable"
    exit 100
fi
B=$(basename $0 .sh)
DL=$1
OUT="${B}_out.txt"
EXP="${B}_expected.txt"

echo '[1,"no-op"]' |
$DL -m -i stdin JSON -o stdout JSON > $OUT

cat > $EXP <<EOF
[1]
EOF

diff -bq $OUT $EXP && rm -f $OUT $EXP && exit 0
exit $?
