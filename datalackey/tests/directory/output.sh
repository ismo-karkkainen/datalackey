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
nap
EOF
chmod a+x _script.sh

(
echo '["1","run","out","JSON","stdout","output","label","mapped","program","./_script.sh"]'
sleep 1
echo '[2,"storage-info"]'
echo '[3,"get","mapped"]'
) | $DL -d "$STORE" -i stdin JSON -o stdout JSON |
replace-pid > $OUT

cat > $EXP <<EOF
["1","run","running","pid"]
[null,"process","started","1","pid"]
["1","run","input","closed"]
[null,"data","stored","mapped",1]
["1","run","exit",0]
["1","run","finished"]
[null,"process","ended","1","pid"]
[2,"storage-info","",{"mapped":{"serial":1,"JSON":7}}]
[3,"get","",{"mapped":"value"}]
EOF

COUT="$(pwd)/td/.datalackey/catalog"
CEXP="${B}_cat_expected.txt"
cat > $CEXP <<EOF
{"mapped":["JSON",1]}
EOF

test 2 -eq $(ls $STORE/.datalackey/ | wc -w) &&
test -f "$STORE/.datalackey/10" &&
compare-output $COUT $CEXP &&
compare-output $OUT $EXP &&
rm -rf $OUT $EXP $COUT $CEXP _script.sh "$STORE"
