#!/bin/bash

if [ $# -ne 2 ]; then
    echo "Usage: $(basename $0) bad-label datalackey-executable"
    exit 100
fi
B=$(basename $0 .sh)
L=$1
DL=$2
OUT="${B}_out.txt"
EXP="${B}_expected.txt"

cat > _script.sh << EOF
#!/bin/sh
echo '{ $L: "value" }'
EOF
chmod a+x _script.sh

(
echo '[1,"run","channel","out","JSON","stdout","output","label","mapped","program","./_script.sh"]'
sleep 1
echo '[2,"list"]'
) | $DL -m -i stdin JSON -o stdout JSON > $OUT
rm -f _script.sh

cat > $EXP <<EOF
[1,"error","format"]
[1,"exit",0]
[1,"finished"]
[2,{}]
EOF

diff -bq $OUT $EXP && rm -f $OUT $EXP && exit 0
