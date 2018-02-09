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
echo '{ "label"'
echo ':'
echo '"value"'
echo '}'
echo '{ "label2": 13 }'
echo '{ "label3": 31 }'
sleep 1
EOF
chmod a+x _script.sh

(
echo '[1,"run","channel","out","JSON","stdout","output-prefix","pre-","output-postfix","-post","output","label","mapped","program","./_script.sh"]'
sleep 2
echo '[2,"list"]'
echo '[3,"get","mapped"]'
echo '[4,"delete","pre-label2-post"]'
echo '[5,"list"]'
) | $DL -d "$STORE" -i stdin JSON -o stdout JSON |
sed 's/"running",.*]$/"running",pid]/' > $OUT

cat > $EXP <<EOF
[1,"running",pid]
[1,"stored","mapped"]
[1,"stored","pre-label2-post"]
[1,"stored","pre-label3-post"]
[1,"exit",0]
[1,"finished"]
[2,{"mapped":{"JSON":7},"pre-label2-post":{"JSON":2},"pre-label3-post":{"JSON":2}}]
[3,{"mapped":"value"}]
[4,"deleted","pre-label2-post"]
[5,{"mapped":{"JSON":7},"pre-label3-post":{"JSON":2}}]
EOF

COUT="$(pwd)/td/.datalackey/catalog"
CEXP="${B}_cat_expected.txt"
cat > $CEXP <<EOF
{"mapped":["JSON",1],"pre-label3-post":["JSON",3]}
EOF

test 3 -eq $(ls $STORE/.datalackey/ | wc -w) &&
test -f "$STORE/.datalackey/10" &&
test -f "$STORE/.datalackey/30" &&
diff -bq $COUT $CEXP &&
diff -bq $OUT $EXP &&
rm -rf $OUT $EXP $COUT $CEXP _script.sh "$STORE"
