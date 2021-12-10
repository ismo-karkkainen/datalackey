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
while read L
do
    echo \$L
done
EOF
chmod a+x _script.sh

(
echo '{"label":123}'
echo '["1","run","in","JSON","stdin","out","JSON","stdout","output-prefix","fed-","program","./_script.sh"]'
nap
echo '[2,"feed","1","input","label","in1"]'
nap
echo '[3,"feed","1","input","label","in2"]'
nap
echo '[4,"end-feed","1"]'
) | $DL -d "$STORE" -i stdin JSON -o stdout JSON |
replace-pid | oneline-keysort-json > $OUT

cat << EOF > $EXP
[null,"data","stored","label",1]
["1","run","running","pid"]
[null,"process","started","1","pid"]
[2,"done",""]
[null,"data","stored","fed-in1",2]
[3,"done",""]
[null,"data","stored","fed-in2",3]
set
[4,"done",""]
["1","run","exit",0]
["1","run","input","closed"]
end
[null,"process","ended","1","pid"]
["1","run","finished"]
["1","done",""]
EOF

COUT="$(pwd)/td/.datalackey/catalog"
CEXP="${B}_cat_expected.txt"
cat > $CEXP <<EOF
{"fed-in1":["JSON",2],"fed-in2":["JSON",3],"label":["JSON",1]}
EOF

test 4 -eq $(ls $STORE/.datalackey/ | wc -w) &&
test -f "$STORE/.datalackey/10" &&
test -f "$STORE/.datalackey/20" &&
test -f "$STORE/.datalackey/30" &&
compare-output $COUT $CEXP &&
compare-output $OUT $EXP &&
rm -rf $OUT $EXP $COUT $CEXP _script.sh "$STORE"
