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
echo '{ "label"'
echo ':'
echo '"value"'
echo '}'
echo '{ "label2": 13 }'
echo '{ "label3": 31 }'
nap
EOF
chmod a+x _script.sh

(
echo '["1","run","out","JSON","stdout","output-prefix","pre-","output-postfix","-post","output","label","mapped","program","./_script.sh"]'
sleep 1
echo '[2,"storage-info"]'
echo '[3,"get","mapped"]'
echo '[4,"delete","pre-label2-post"]'
echo '[5,"storage-info"]'
) | $DL -m -i stdin JSON -o stdout JSON |
sed 's/"running",.*]$/"running",pid]/' > $OUT

cat > $EXP <<EOF
["1","run","running",pid]
["1","run","input","closed"]
["1","data","stored",{"mapped":1}]
["1","data","stored",{"pre-label2-post":2}]
["1","data","stored",{"pre-label3-post":3}]
["1","run","exit",0]
["1","run","finished"]
[2,"storage-info","",{"mapped":{"serial":1,"JSON":7},"pre-label2-post":{"serial":2,"JSON":2},"pre-label3-post":{"serial":3,"JSON":2}}]
[3,"get","",{"mapped":"value"}]
[4,"delete","deleted","pre-label2-post"]
[5,"storage-info","",{"mapped":{"serial":1,"JSON":7},"pre-label3-post":{"serial":3,"JSON":2}}]
EOF

compare-output $OUT $EXP && rm -f $OUT $EXP _script.sh
