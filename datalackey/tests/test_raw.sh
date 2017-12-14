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
for w in "a" "ab" "abc"
do
    echo "\$w"
    sleep 1
done
EOF
chmod a+x _script.sh

echo '[1,"run","channel","out","raw","stdout","program","./_script.sh"]' |
$DL -m -i stdin JSON -o stdout JSON > $OUT
rm -f _script.sh

cat > $EXP <<EOF
[1,97,10]
[1,97,98,10]
[1,97,98,99,10]
[1,"exit",0]
[1,"finished"]
EOF

diff -bq $OUT $EXP && rm -f $OUT $EXP
