#!/bin/sh

if [ $# -ne 1 ]; then
    echo "Usage: $(basename $0) datalackey-executable"
    exit 100
fi
B=$(basename $0 .sh)
DL=$1
OUT="${B}_out.txt"
EXP="${B}_expected.txt"

(
echo '{"label":1234,"label2":13}{"ignored":1234,"label2:13"}'
printf '\0'
echo '[1,"rename","label","name","miss","unused",4,"unused-too","label2","name2","miss2","unused"]'
echo '[2,"rename","label","name","miss","unused","label2","name2","miss2","unused"]'
) | $DL -m -i stdin JSON -o stdout JSON > $OUT

cat > $EXP <<EOF
[null,"data","stored","label",1]
[null,"data","stored","label2",2]
[null,"error","format"]
[null,"channel","reset"]
[1,"error","not-string",1,"rename","label","name","miss","unused",4]
[1,"done",""]
[null,"data","deleted","label",1]
[null,"data","stored","name",3]
[null,"data","deleted","label2",2]
[null,"data","stored","name2",4]
[2,"rename","missing","miss","miss2"]
[2,"done",""]
EOF

compare-output $OUT $EXP && rm -f $OUT $EXP
