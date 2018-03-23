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
while true
do
    echo
done
EOF
chmod a+x _script.sh

(
echo '[1,"run","program","./_script.sh"]'
sleep 1
echo '[2,"terminate",1]'
sleep 1
echo '[3,"processes"]'
) | $DL -m -i stdin JSON -o stdout JSON |
sed 's/"running",.*]$/"running",pid]/' > $OUT

cat > $EXP <<EOF
[1,"run","running",pid]
[2,"terminate","terminated",1]
[1,"run","signal",9]
[1,"run","finished"]
[3,"processes","",{}]
EOF

diff -bq $OUT $EXP && rm -f $OUT $EXP _script.sh
