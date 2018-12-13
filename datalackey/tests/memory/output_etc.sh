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
replace-pid | oneline-keysort-json > $OUT

cat > $EXP <<EOF
["1","run","running","pid"]
[null,"process","started","1","pid"]
["1","run","input","closed"]
[null,"data","stored","mapped",1]
[null,"data","stored","pre-label2-post",2]
[null,"data","stored","pre-label3-post",3]
["1","run","exit",0]
["1","run","finished"]
[null,"process","ended","1","pid"]
[2,"storage-info","",{"mapped":{"JSON":7,"serial":1},"pre-label2-post":{"JSON":2,"serial":2},"pre-label3-post":{"JSON":2,"serial":3}}]
[3,"get","",{"mapped":"value"}]
[null,"data","deleted","pre-label2-post",1]
[5,"storage-info","",{"mapped":{"JSON":7,"serial":1},"pre-label3-post":{"JSON":2,"serial":3}}]
EOF

compare-output $OUT $EXP && rm -f $OUT $EXP _script.sh
