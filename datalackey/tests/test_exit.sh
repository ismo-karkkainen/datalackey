#!/bin/bash

if [ $# -ne 2 ]; then
    echo "Usage: $(basename $0) code datalackey-executable"
    exit 100
fi
B=$(basename $0 .sh)
C=$1
DL=$2
OUT="${B}_out.txt"
EXP="${B}_expected.txt"

cat > _script.sh << EOF
#!/bin/sh
exit $1
EOF
chmod a+x _script.sh

echo "[1,\"run\",\"program\",\"./_script.sh\",$C]" |
$DL -m -i stdin JSON -o stdout JSON > $OUT
rm -f _script.txt

cat > $EXP <<EOF
[1,"exit",$C]
[1,"finished"]
EOF

diff -bq $OUT $EXP && rm -f $OUT $EXP && exit 0
exit $?
