#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Usage: $(basename $0) datalackey-executable"
    exit 100
fi
B=$(basename $0 .sh)
DL=$1
OUT="${B}_out.txt"
EXP="${B}_expected.txt"

cat > _script.sh << EOF
#!/bin/sh
while true
do
    echo
done
EOF
chmod a+x _script.sh

(
echo '[null,"run","program","./_script.sh"]'
nap
echo '[null,"terminate",null]'
sleep 1
echo '[null,"processes"]'
) | $DL -m -i stdin JSON -o stdout JSON |
sed 's/"running",.*]$/"running",pid]/' > $OUT

cat > $EXP <<EOF
EOF

compare-output $OUT $EXP && rm -f $OUT $EXP _script.sh
