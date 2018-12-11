#!/bin/bash

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
sleep 1
EOF
chmod a+x _script.sh

(
echo '["1","run","program","./_script.sh"]'
nap
kill -s SIGSTOP $(cat $PID)
sleep 1
kill -s SIGCONT $(cat $PID)
) | $DL -m -i stdin JSON -o stdout JSON |
sed 's/"running",.*]$/"running","pid"]/' > $OUT

cat > $EXP <<EOF
["1","run","running","pid"]
["1","run","input","closed"]
["1","run","stopped",17]
["1","run","continued"]
["1","run","exit",0]
["1","run","finished"]
EOF

compare-output $OUT $EXP && rm -f $OUT $EXP _script.sh $PID
