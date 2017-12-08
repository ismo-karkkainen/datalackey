#!/bin/bash

if [ $# -ne 2 ]; then
    echo "Usage: $(basename $0) signal datalackey-executable"
    exit 100
fi
S=$1
DL=$2

cat > _script.sh << EOF
#!/bin/sh
kill -$1 \$\$
EOF
chmod a+x _script.sh

echo "[1,\"run\",\"program\",\"./_script.sh\",$S]" |
$DL -m -i stdin JSON -o stdout JSON > _out.txt
cat > _expected.txt <<EOF
[1,"signal",$S][1,"finished"]
EOF
diff -bq _out.txt _expected.txt
RV=$?
rm -f _script.sh _expected.txt _out.txt
exit $RV
