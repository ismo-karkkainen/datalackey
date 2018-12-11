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
echo '{"item":"value","item2":12345}'
nap
echo '["s1","rename","item","name","item2","name2"]'
echo '["s2","delete","name","name2"]'
EOF
chmod a+x _script.sh

(
echo '["1","run","out","JSON","stdout","program","./_script.sh"]'
) | $DL -m -i stdin JSON -o stdout JSON |
sed 's/"running",.*]$/"running","pid"]/' > "$OUT"

cat > "$EXP" << EOF
["1","run","running","pid"]
["1","run","input","closed"]
["1","data","stored",{"item":1,"item2":2}]
["1","run","exit",0]
[null,"data","renamed",{"item":null,"item2":null,"name":3,"name2":4}]
[null,"data","deleted","name","name2"]
["1","run","finished"]
EOF

compare-output "$OUT" "$EXP" && rm -f "$OUT" "$EXP" _script.sh
