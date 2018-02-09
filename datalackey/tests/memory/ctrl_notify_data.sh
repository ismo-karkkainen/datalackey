#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Usage: $(basename $0) datalackey-executable"
    exit 100
fi
B=$(basename $0 .sh)
DL=$1
OUT="${B}_out.txt"
EXP="${B}_expected.txt"
CTOUT="${B}_controller_out.txt"
CTEXP="${B}_controller_expected.txt"

cat > _script.sh << EOF
#!/bin/sh
echo '{ "label": "value" }'
EOF
chmod a+x _script.sh

cat > _controller.sh << EOF
#!/bin/sh
echo '["a","run","channel","out","JSON","stdout","notify","none","program","./_script.sh"]'
cat | sed 's/"running",.*]$/"running",pid]/' > $CTOUT
EOF
chmod a+x _controller.sh

(
echo '[1,"run","channel","out","JSON","stdout","channel","in","JSON","stdin","notify","data","program","./_controller.sh"]'
sleep 1
echo '[2,"end-feed",1]'
sleep 1
echo '[3,"get","label"]'
) | $DL -m -i stdin JSON -o stdout JSON |
sed 's/"running",.*]$/"running",pid]/' > $OUT

cat > $EXP <<EOF
[1,"running",pid]
[null,"stored","label"]
[2,"ended",1]
[1,"exit",0]
[1,"input","closed"]
[1,"finished"]
[3,{"label":"value"}]
EOF

cat > $CTEXP <<EOF
["a","running",pid]
["a","exit",0]
["a","stored","label"]
["a","finished"]
EOF

diff -bq $OUT $EXP && diff -bq $CTOUT $CTEXP && rm -f $OUT $EXP $CTOUT $CTEXP _script.sh _controller.sh
