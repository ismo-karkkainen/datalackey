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
sleep 1
echo '[null,"terminate",null]'
echo '[null,"wait",null]'
echo '[null,"processes"]'
) | $DL -m -i stdin JSON -o stdout JSON > $OUT
rm -f _script.sh

cat > $EXP <<EOF
EOF

diff -bq $OUT $EXP && rm -f $OUT $EXP && exit 0
