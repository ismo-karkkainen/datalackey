#!/bin/sh

if [ $# -ne 4 ]; then
    echo "Usage: $(basename $0) loop-count item-count size datalackey-executable"
    exit 100
fi
B=$(basename $0 .sh)
N=$1
COUNT=$2
SIZE=$3
DL=$4
OUT="${B}_out.txt"
EXP="${B}_expected.txt"

cat > _script.sh << EOF
#!/bin/sh
size=\$2
echo '{"data'\$1'":[0'
while [ \$size -gt 0 ]
do
    echo ",\$size,\$size,\$size,\$size,\$size,\$size,\$size,\$size,\$size,\$size"
    size=\$((size-10))
done
echo ']}'
EOF
chmod a+x _script.sh

(
ID=0
while [ $N -gt 0 ]
do
    K=$COUNT
    while [ $K -gt 0 ]
    do
        echo '['$ID',"run","out","JSON","stdout","program","./_script.sh",'$K','$SIZE']'
        ID=$((ID+1))
        K=$((K-1))
        # Run sequentially for starters.
        nap
        while [ $(ps | grep _script.sh | grep -v grep | wc -w) -gt 0 ]
        do
            nap
        done
    done
    N=$((N-1))
done
) | $DL -m -i stdin JSON -o stdout JSON |
grep '"stored"' |
awk -F, '{ d[$(NF - 1)] = $NF } END { for (x in d) print x, d[x] }' |
sort > $OUT


(
K=$((N*COUNT-COUNT+1))
while [ $COUNT -gt 0 ]
do
    echo '"data'$COUNT'" '$K']'
    COUNT=$((COUNT-1))
    K=$((K+1))
done
) | sort > $EXP

compare-output $OUT $EXP && rm -f $OUT $EXP _script.sh
