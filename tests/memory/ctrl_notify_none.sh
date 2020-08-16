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
) | $DL -m -i stdin JSON -o stdout JSON |
replace-pid > $OUT

cat > $EXP <<EOF
["1","run","running","pid"]
[null,"process","started","1","pid"]
["1","run","input","closed"]
[null,"process","started","a","pid"]
set
[null,"process","ended","a","pid"]
[null,"data","stored","label",1]
end
["1","run","exit",0]
[null,"process","ended","1","pid"]
["1","run","finished"]
["1","done",""]
[2,"get","",{"label":"value"}]
[2,"done",""]
EOF

compare-output $OUT $EXP && rm -f $OUT $EXP _script.sh _controller.sh
