#!/bin/bash

if [ $# -ne 2 ]; then
    echo "Usage: $(basename $0) signal datalackey-executable"
    exit 100
fi
B=$(basename $0 .sh)
S=$1
DL=$2
OUT="${B}_out.txt"
EXP="${B}_expected.txt"

cat > _script.sh << EOF
#!/bin/sh
kill -$1 \$\$
EOF
chmod a+x _script.sh

echo "[1,\"run\",\"program\",\"./_script.sh\",$S]" |
$DL -m -i stdin JSON -o stdout JSON |
sed 's/"running",.*]$/"running",pid]/' > $OUT

cat > $EXP <<EOF
[1,"run","running",pid]
[1,"run","signal",$S]
[1,"run","finished"]
EOF

diff -bq $OUT $EXP && rm -f $OUT $EXP _script.sh
