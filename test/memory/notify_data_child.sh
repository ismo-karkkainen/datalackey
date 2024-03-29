#!/bin/sh

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
replace-pid > "$OUT"

cat > "$EXP" << EOF
["1","run","running","pid"]
[null,"process","started","1","pid"]
set
["1","run","input","closed"]
[null,"data","stored","item",1]
[null,"data","stored","item2",2]
end
set
["1","run","exit",0]
[null,"data","deleted","item",1]
[null,"data","stored","name",3]
[null,"data","deleted","item2",2]
[null,"data","stored","name2",4]
[null,"data","deleted","name",3]
[null,"data","deleted","name2",4]
end
[null,"process","ended","1","pid"]
["1","run","finished"]
["1","done",""]
EOF

compare-output "$OUT" "$EXP" && rm -f "$OUT" "$EXP" _script.sh
