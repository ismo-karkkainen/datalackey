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
touch cwd
EOF
chmod a+x _script.sh

(
echo '[1,"run","channel","out","JSON","stdout","change-directory","invalid","program","./_script.sh"]'
) | $DL -m -i stdin JSON -o stdout JSON |
sed 's/"running",.*]$/"running",pid]/' > $OUT

cat > $EXP <<EOF
[1,"run","error","change-directory","invalid","No such file or directory"]
EOF

compare-output $OUT $EXP && rm -f $OUT $EXP _script.sh
