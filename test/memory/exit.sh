#!/bin/sh

if [ $# -ne 2 ]; then
    echo "Usage: $(basename $0) code datalackey-executable"
    exit 100
fi
B=$(basename $0 .sh)
C=$1
DL=$2
OUT="${B}_out.txt"
EXP="${B}_expected.txt"

cat > _script.sh << EOF
#!/bin/sh
exit \$1
EOF
chmod a+x _script.sh

echo "[\"1\",\"run\",\"program\",\"./_script.sh\",$C]" |
$DL -m -i stdin JSON -o stdout JSON | replace-pid > $OUT

cat > $EXP <<EOF
["1","run","running","pid"]
[null,"process","started","1","pid"]
set
["1","run","input","closed"]
["1","run","exit",$C]
end
[null,"process","ended","1","pid"]
["1","run","finished"]
["1","done",""]
EOF

compare-output $OUT $EXP && rm -f $OUT $EXP _script.sh
