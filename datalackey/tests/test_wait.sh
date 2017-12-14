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
sleep 1
EOF
chmod a+x _script.sh

(
echo '[null,"run","program","./_script.sh"]'
echo '[1,"wait",null,2]'
echo '[null,"processes"]'
) | $DL -m -i stdin JSON -o stdout JSON > $OUT
rm -f _script.sh

cat > $EXP <<EOF
[1,"missing",2]
[1,"terminated",null]
EOF

diff -bq $OUT $EXP && rm -f $OUT $EXP
