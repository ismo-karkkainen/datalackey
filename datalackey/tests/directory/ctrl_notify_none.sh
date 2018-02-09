#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Usage: $(basename $0) datalackey-executable"
    exit 100
fi
B=$(basename $0 .sh)
DL=$1
OUT="${B}_out.txt"
EXP="${B}_expected.txt"
STORE="$(pwd)/td"

rm -rf "$STORE"
mkdir "$STORE"

cat > _script.sh << EOF
#!/bin/sh
echo '{ "label": "value" }'
EOF
chmod a+x _script.sh

cat > _controller.sh << EOF
#!/bin/sh
echo '["a","run","channel","out","JSON","stdout","program","./_script.sh"]'
sleep 1
EOF
chmod a+x _controller.sh

(
echo '[1,"run","channel","out","JSON","stdout","notify","none","end-feed","program","./_controller.sh"]'
sleep 2
echo '[2,"get","label"]'
) | $DL -d "$STORE" -i stdin JSON -o stdout JSON |
sed 's/"running",.*]$/"running",pid]/' > $OUT

cat > $EXP <<EOF
[1,"running",pid]
[null,"stored","label"]
[1,"exit",0]
[1,"finished"]
[2,{"label":"value"}]
EOF

COUT="$(pwd)/td/.datalackey/catalog"
CEXP="${B}_cat_expected.txt"
cat > $CEXP <<EOF
{"label":["JSON",1]}
EOF

test 2 -eq $(ls $STORE/.datalackey/ | wc -w) &&
test -f "$STORE/.datalackey/10" &&
diff -bq $COUT $CEXP &&
diff -bq $OUT $EXP &&
rm -rf $OUT $EXP _script.sh _controller.sh $COUT $CEXP "$STORE"
