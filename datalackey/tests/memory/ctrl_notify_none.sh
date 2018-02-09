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
echo '{ "label": "value" }'
EOF
chmod a+x _script.sh

cat > _controller.sh << EOF
#!/bin/sh
echo '["a","run","channel","out","JSON","stdout","program","./_script.sh"]'
sleep 1
EOF
chmod a+x _controller.sh

(
echo '[1,"run","channel","out","JSON","stdout","notify","none","end-feed","program","./_controller.sh"]'
sleep 2
echo '[2,"get","label"]'
) | $DL -m -i stdin JSON -o stdout JSON |
sed 's/"running",.*]$/"running",pid]/' > $OUT

cat > $EXP <<EOF
[1,"running",pid]
[null,"stored","label"]
[1,"exit",0]
[1,"finished"]
[2,{"label":"value"}]
EOF

diff -bq $OUT $EXP && rm -f $OUT $EXP _script.sh _controller.sh
