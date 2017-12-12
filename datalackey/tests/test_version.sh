#!/bin/bash

if [ $# -ne 2 ]; then
    echo "Usage: $(basename $0) version-header datalackey-executable"
    exit 100
fi
B=$(basename $0 .sh)
VH=$1
DL=$2
OUT="${B}_out.txt"
EXP="${B}_expected.txt"

echo '[1,"version"]' |
$DL -m -i stdin JSON -o stdout JSON > $OUT

cat > $EXP <<EOF
[1,{"version":$(cat $VH | grep "Version " | awk '{ printf("%s", $NF) }' | sed 's/;//')}]
EOF

diff -bq $OUT $EXP && rm -f $OUT $EXP && exit 0
exit $?
