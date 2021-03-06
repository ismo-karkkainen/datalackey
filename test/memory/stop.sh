#!/bin/sh

if [ $# -ne 1 ]; then
    echo "Usage: $(basename $0) datalackey-executable"
    exit 100
fi
B=$(basename $0 .sh)
DL=$1
OUT="${B}_out.txt"
EXP="${B}_expected.txt"
PID="${B}_pid.txt"

cat > _script.sh << EOF
#!/bin/sh
echo \$\$ > $PID
sleep 2
EOF
chmod a+x _script.sh

(
echo '["1","run","program","./_script.sh"]'
nap
kill -s STOP $(cat $PID)
sleep 1
kill -s CONT $(cat $PID)
) | $DL -m -i stdin JSON -o stdout JSON |
replace-pid > $OUT

STOP=17
if [ "$(uname)" = "Linux" ]; then
    STOP=19
fi

cat > $EXP <<EOF
["1","run","running","pid"]
[null,"process","started","1","pid"]
["1","run","input","closed"]
["1","run","stopped",$STOP]
["1","run","continued"]
["1","run","exit",0]
[null,"process","ended","1","pid"]
["1","run","finished"]
["1","done",""]
EOF

compare-output $OUT $EXP && rm -f $OUT $EXP _script.sh $PID
