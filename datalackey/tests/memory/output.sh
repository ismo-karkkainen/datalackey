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

(
echo '[1,"run","channel","out","JSON","stdout","output","label","mapped","program","./_script.sh"]'
nap
echo '[2,"storage-info"]'
echo '[3,"get","mapped","miss",4]'
) | $DL -m -i stdin JSON -o stdout JSON |
sed 's/"running",.*]$/"running",pid]/' > $OUT

cat > $EXP <<EOF
[1,"run","running",pid]
[1,"run","input","closed"]
set
[1,"data","stored","mapped"]
[1,"run","exit",0]
[1,"run","finished"]
end
[2,"storage-info","",{"mapped":{"JSON":7}}]
[3,"get","invalid",4]
[3,"get","missing","miss"]
[3,"get","",{"mapped":"value"}]
EOF

compare-output $OUT $EXP && rm -f $OUT $EXP _script.sh
