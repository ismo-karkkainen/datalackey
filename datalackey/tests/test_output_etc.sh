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
echo '[ "message in a\\\\nscript"'
echo ']'
echo '{ "label3": 31 }'
EOF
chmod a+x _script.sh

(
echo '[1,"run","channel","out","JSON","stdout","output-prefix","pre-","output-postfix","-post","output","label","mapped","program","./_script.sh"]'
sleep 1
echo '[2,"list"]'
echo '[3,"get","mapped"]'
echo '[4,"delete","pre-label2-post"]'
echo '[5,"list"]'
) | $DL -m -i stdin JSON -o stdout JSON > $OUT
rm -f _script.sh

cat > $EXP <<EOF
[1,"stored","mapped","JSON"]
[1,"stored","pre-label2-post","JSON"]
[1,["message in a\\nscript"]]
[1,"stored","pre-label3-post","JSON"]
[1,"exit",0]
[1,"finished"]
[2,{"mapped":{"JSON":7},"pre-label2-post":{"JSON":2},"pre-label3-post":{"JSON":2}}]
[3,{"mapped":"value"}]
[4,"deleted","pre-label2-post"]
[5,{"mapped":{"JSON":7},"pre-label3-post":{"JSON":2}}]
EOF

diff -bq $OUT $EXP && rm -f $OUT $EXP
