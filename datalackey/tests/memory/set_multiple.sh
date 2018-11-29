#!/bin/bash

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
) | $DL -m -i stdin JSON -o stdout JSON > $OUT

cat > $EXP <<EOF
[null,"data","stored","label","label2"]
[null,"error","format"]
[null,"channel","reset"]
[1,"rename","invalid",4]
[1,"rename","missing","miss","miss2"]
[1,"rename","renamed","label","name","label2","name2"]
EOF

compare-output $OUT $EXP && rm -f $OUT $EXP
