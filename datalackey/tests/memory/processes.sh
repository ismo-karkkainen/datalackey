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
sleep 1
EOF
chmod a+x _script.sh

(
echo '["1","run","program","./_script.sh"]'
nap
ps u | grep _script.sh | grep -v grep | awk '{ print $2 }' > _pid.txt
sleep 1
) | $DL -m -i stdin JSON -o stdout JSON > $OUT

cat > $EXP <<EOF
["1","run","running",$(cat _pid.txt)]
[null,"process","started","1",$(cat _pid.txt)]
["1","run","input","closed"]
["1","run","exit",0]
[null,"process","ended","1",$(cat _pid.txt)]
["1","run","finished"]
["1","done",""]
EOF

compare-output $OUT $EXP && rm -f $OUT $EXP _script.sh _pid.txt
