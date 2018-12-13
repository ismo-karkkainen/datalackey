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
for w in "a" "ab" "abc"
do
    echo "\$w" 1>&2
    nap
done
EOF
chmod a+x _script.sh

echo '["1","run","out","bytes","stderr","program","./_script.sh"]' |
$DL -m -i stdin JSON -o stderr JSON 2>&1 |
replace-pid > $OUT

cat > $EXP <<EOF
["1","run","running","pid"]
[null,"process","started","1","pid"]
["1","run","input","closed"]
["1","run","bytes",97,10]
["1","run","bytes",97,98,10]
["1","run","bytes",97,98,99,10]
["1","run","exit",0]
["1","run","finished"]
[null,"process","ended","1","pid"]
EOF

compare-output $OUT $EXP && rm -f $OUT $EXP _script.sh
