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
while read L
do
    echo \$L
done
nap
EOF
chmod a+x _script.sh

(
echo '{"label":123}'
echo '[1,"run","channel","in","JSON","stdin","channel","out","JSON","stdout","output-prefix","fed-","program","./_script.sh"]'
nap
echo '[2,"feed",1,"input","label","in1"]'
nap
echo '[3,"feed",1,"input","label","in2"]'
nap
echo '[4,"end-feed",1]'
sleep 1
echo '[5,"storage-info"]'
) | $DL -m -i stdin JSON -o stdout JSON |
sed 's/"running",.*]$/"running",pid]/' > $OUT

cat > $EXP <<EOF
[null,"data","stored","label"]
[1,"run","running",pid]
[1,"data","stored","fed-in1"]
[1,"data","stored","fed-in2"]
[4,"end-feed","",1]
[1,"run","input","closed"]
[1,"run","exit",0]
[1,"run","finished"]
[5,"storage-info","",{"fed-in1":{"JSON":3},"fed-in2":{"JSON":3},"label":{"JSON":3}}]
EOF

compare-output $OUT $EXP && rm -f $OUT $EXP _script.sh
