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
echo '{ "label": "value" }'
EOF
chmod a+x _script.sh

(
echo '[1,"run","channel","out","JSON","stdout","output","label","mapped","program","./_script.sh"]'
sleep 1
echo '[2,"list"]'
echo '[3,"get","mapped"]'
) | $DL -m -i stdin JSON -o stdout JSON > $OUT
rm -f _script.sh

cat > $EXP <<EOF
[1,"exit",0]
[1,"finished"]
[2,{"mapped":{"JSON":7}}]
[3,{"mapped":"value"}]
EOF

diff -bq $OUT $EXP && rm -f $OUT $EXP && exit 0
exit $?
