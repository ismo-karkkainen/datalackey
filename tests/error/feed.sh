#!/bin/sh

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
    echo \$L | replace-pid >> $OUT_SCR
done
nap
EOF
chmod a+x _script.sh

(
echo '["1","run","in","JSON","stdin","out","JSON","stdout","notify","data","program","./_script.sh"]'
nap
echo '[2,"feed",0]'
echo '[5,"feed","1","invalid"]'
echo '[6,"feed","1","input"]'
echo '[7,"feed","1","input",7,8]'
echo '[8,"feed","1","input","name",8]'
echo '[9,"feed","1","input","name","child","direct",9]'
echo '[10,"feed","1","input","name","child","direct","dir",10]'
echo '[11,"feed","1","input","name","child","direct","dir","child"]'
echo '[12,"feed","1","input","name","child","direct","dir","child2"]'
echo '{"name":0}'
echo '[3,"end-feed","1"]'
echo '[4,"feed","1"]'
) | $DL -m -i stdin JSON -o stdout JSON |
replace-pid > "$OUT"

cat > "$EXP" << EOF
["1","run","running","pid"]
[null,"process","started","1","pid"]
[null,"process","started","sub","pid"]
[null,"data","stored","item",1]
[null,"data","stored","item2",2]
[null,"process","ended","sub","pid"]
[2,"feed","error","not-found"]
[2,"done",""]
[5,"error","unknown",5,"feed","1","invalid"]
[5,"done",""]
[6,"error","missing",6,"feed","1","input"]
[6,"done",""]
[7,"error","not-string",7,"feed","1","input",7]
[7,"done",""]
[8,"error","not-string",8,"feed","1","input","name",8]
[8,"done",""]
[9,"error","missing",9,"feed","1","input","name","child","direct",9]
[9,"done",""]
[10,"error","not-string",10,"feed","1","input","name","child","direct","dir",10]
[10,"done",""]
[11,"feed","error","direct","duplicate","child"]
[11,"done",""]
[12,"feed","error","missing","name"]
[12,"done",""]
[null,"data","stored","name",3]
[3,"done",""]
set
["1","run","input","closed"]
[4,"feed","error","closed"]
[4,"done",""]
end
["1","run","exit",0]
[null,"process","ended","1","pid"]
["1","run","finished"]
["1","done",""]
EOF

cat > "$EXP_SCR" << EOF
["sub","run","running","pid"]
["sub","run","input","closed"]
set
[null,"data","stored","item",1]
[null,"data","stored","item2",2]
["sub","run","exit",0]
["sub","run","finished"]
["sub","done",""]
end
[null,"data","stored","name",3]
EOF

compare-output "$OUT" "$EXP" && compare-output "$OUT_SCR" "$EXP_SCR" && rm -f "$OUT" "$EXP" "$OUT_SCR" "$EXP_SCR" _script.sh _subscript.sh
