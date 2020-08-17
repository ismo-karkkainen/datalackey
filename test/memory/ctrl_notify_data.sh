#!/bin/sh

if [ $# -ne 1 ]; then
    echo "Usage: $(basename $0) datalackey-executable"
    exit 100
fi
B=$(basename $0 .sh)
DL=$1
OUT="${B}_out.txt"
EXP="${B}_expected.txt"
CTOUT="${B}_controller_out.txt"
CTEXP="${B}_controller_expected.txt"

cat > _script.sh << EOF
#!/bin/sh
echo '{ "label": "value" }'
EOF
chmod a+x _script.sh

cat > _controller.sh << EOF
#!/bin/sh
echo '["a","run","out","JSON","stdout","program","./_script.sh"]'
cat | replace-pid > $CTOUT
EOF
chmod a+x _controller.sh

(
echo '[1,"run","out","JSON","stdout","in","JSON","stdin","notify","data","program","./_controller.sh"]'
nap
echo '[2,"end-feed",1]'
nap
echo '[3,"get","label"]'
) | $DL -m -i stdin JSON -o stdout JSON |
replace-pid > $OUT

cat > $EXP <<EOF
[1,"run","running","pid"]
[null,"process","started",1,"pid"]
[null,"process","started","a","pid"]
set
[null,"process","ended","a","pid"]
[null,"data","stored","label",1]
[2,"done",""]
[1,"run","exit",0]
[1,"run","input","closed"]
end
[null,"process","ended",1,"pid"]
[1,"run","finished"]
[1,"done",""]
[3,"get","",{"label":"value"}]
[3,"done",""]
EOF

cat > $CTEXP <<EOF
["a","run","running","pid"]
["a","run","input","closed"]
["a","run","exit",0]
[null,"data","stored","label",1]
["a","run","finished"]
["a","done",""]
EOF

compare-output $OUT $EXP && compare-output $CTOUT $CTEXP && rm -f $OUT $EXP $CTOUT $CTEXP _script.sh _controller.sh
