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

SUBDIR=$(pwd)/subdir
rm -rf "$SUBDIR"
mkdir "$SUBDIR"

(
echo '[1,"run","channel","out","JSON","stdout","change-directory",'
echo "\"$SUBDIR\""
echo ',"program","./_script.sh"]'
) | $DL -m -i stdin JSON -o stdout JSON |
sed 's/"running",.*]$/"running",pid]/' > $OUT

cat > $EXP <<EOF
[1,"run","running",pid]
[1,"run","input","closed"]
[1,"run","exit",0]
[1,"run","finished"]
EOF

test -f "$SUBDIR/cwd" &&
diff -bq $OUT $EXP && rm -rf $OUT $EXP _script.sh "$SUBDIR"
