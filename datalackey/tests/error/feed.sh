#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Usage: $(basename $0) datalackey-executable"
    exit 100
fi
B=$(basename $0 .sh)
DL=$1
OUT="${B}_out.txt"
EXP="${B}_expected.txt"
OUT_SCR="${B}_script_out.txt"
EXP_SCR="${B}_script_expected.txt"

cat > _subscript.sh << EOF
#!/bin/sh
echo '{"item":"value","item2":12345}'
EOF
chmod a+x _subscript.sh

cat > _script.sh << EOF
#!/bin/sh
echo '["sub","run","end-feed","out","JSON","stdout","program","./_subscript.sh"]'
rm -f $OUT_SCR
while read L
do
    echo \$L | sed 's/"running",.*]$/"running",pid]/' >> $OUT_SCR
done
nap
EOF
chmod a+x _script.sh

(
echo '[1,"run","in","JSON","stdin","out","JSON","stdout","notify","data","program","./_script.sh"]'
nap
echo '[2,"feed",0]'
echo '[5,"feed",1,"invalid"]'
echo '[6,"feed",1,"input"]'
echo '[7,"feed",1,"input",7,8]'
echo '[8,"feed",1,"input","name",8]'
echo '[9,"feed",1,"input","name","child","direct",9]'
echo '[10,"feed",1,"input","name","child","direct","dir",10]'
echo '[11,"feed",1,"input","name","child","direct","dir","child"]'
echo '[12,"feed",1,"input","name","child","direct","dir","child2"]'
echo '{"name":0}'
echo '[3,"end-feed",1]'
echo '[4,"feed",1]'
) | $DL -m -i stdin JSON -o stdout JSON |
sed -e 's/"running",.*]$/"running",pid]/' -e 's/"sub",.*]$/"sub",pid]/' > "$OUT"

cat > "$EXP" << EOF
[1,"run","running",pid]
[null,"process","started","sub",pid]
[null,"data","stored","item","item2"]
[null,"process","ended","sub",pid]
[2,"feed","error","not-found"]
[5,"feed","error","invalid","argument","unknown"]
[6,"feed","error","input","argument","missing"]
[7,"feed","error","input","argument","not-string"]
[8,"feed","error","input","argument","not-string"]
[9,"feed","error","direct","argument","missing"]
[10,"feed","error","direct","argument","not-string"]
[11,"feed","error","direct","duplicate","child"]
[12,"feed","error","missing","name"]
[null,"data","stored","name"]
[3,"end-feed","",1]
set
[1,"run","input","closed"]
[4,"feed","error","closed"]
end
[1,"run","exit",0]
[1,"run","finished"]
EOF

cat > "$EXP_SCR" << EOF
["sub","run","running",pid]
["sub","run","input","closed"]
set
["sub","data","stored","item","item2"]
["sub","run","exit",0]
["sub","run","finished"]
end
[null,"data","stored","name"]
EOF

compare-output "$OUT" "$EXP" && compare-output "$OUT_SCR" "$EXP_SCR" && rm -f "$OUT" "$EXP" "$OUT_SCR" "$EXP_SCR" _script.sh _subscript.sh
