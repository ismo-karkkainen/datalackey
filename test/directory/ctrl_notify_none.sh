#!/bin/sh

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
echo '["a","run","out","JSON","stdout","program","./_script.sh"]'
nap
EOF
chmod a+x _controller.sh

(
echo '["1","run","out","JSON","stdout","program","./_controller.sh"]'
sleep 1
echo '[2,"get","label"]'
) | $DL -d "$STORE" -i stdin JSON -o stdout JSON |
replace-pid > $OUT

cat > $EXP <<EOF
["1","run","running","pid"]
[null,"process","started","1","pid"]
["1","run","input","closed"]
[null,"process","started","a","pid"]
set
[null,"data","stored","label",1]
[null,"process","ended","a","pid"]
end
["1","run","exit",0]
[null,"process","ended","1","pid"]
["1","run","finished"]
["1","done",""]
[2,"get","",{"label":"value"}]
[2,"done",""]
EOF

COUT="$(pwd)/td/.datalackey/catalog"
CEXP="${B}_cat_expected.txt"
cat > $CEXP <<EOF
{"label":["JSON",1]}
EOF

test 2 -eq $(ls $STORE/.datalackey/ | wc -w) &&
test -f "$STORE/.datalackey/10" &&
compare-output $COUT $CEXP &&
compare-output $OUT $EXP &&
rm -rf $OUT $EXP _script.sh _controller.sh $COUT $CEXP "$STORE"
