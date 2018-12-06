#!/bin/bash

if [ $# -ne 2 ]; then
    echo "Usage: $(basename $0) version-header datalackey-executable"
    exit 100
fi
B=$(basename $0 .sh)
VH=$1
DL=$2
OUT="${B}_out.txt"
EXP="${B}_expected.txt"

echo '[1,"version"]' |
$DL -m -i stdin JSON -o stdout JSON > $OUT

VER=$(cat $VH | grep "Version " | sed 's/;//' | awk '{ printf("%s", $NF) }')
INT=$(cat $VH | grep "Interface " | sed 's/;//' | awk '{ printf("%s", $NF) }')

cat > $EXP <<EOF
[1,"version","",{"datalackey":$VER,"interface":$INT,"commands":["id","no-op"],["id","list"]:["id","storage-info"],["id","get","string,..."]:["id","delete","string,..."],["id","rename","string,string,..."]:["id","version"],{"run":["id","<run>","<subcommands>"],"<subcommands>":[["<subcommand>","<subcommands>"],["<subcommand>"]],"<subcommand>":["<input>","<direct>","<env>","<env-clear>","<output>","<output-prefix>","<output-postfix>","<in>","<out>","<notify>","<end-feed>","<change-directory>","<program>"],"<input>":["input","source-label","target-name"],"<direct>":["direct","value-string|integer|null","target-name"],"<env>":["env","variable-name","value-string|integer|null"],"<env-clear>":["env-clear"],"<output>":["output","result-name","label|null"],"<output-prefix>":["output-prefix","prefix-for-unmapped-result-name"],"<output-postfix>":["output-postfix","unmapped-result-name-postfix"],"<in>":["in","JSON","stdin"],"<out>":["out","JSON|bytes","stdout|stderr"],"<notify>":["notify","data|process"],"<end-feed>":["end-feed"],"<change-directory>":["change-directory","directory"],"<program>":["program","executable"]}:{"feed":["id","<feed>","process-id","<subcommands>"],"<subcommands>":[["<subcommand>","<subcommands>"],["<subcommand>"]],"<subcommand>":["<input>","<direct>"],"<input>":["input","source-label","target-name"],"<direct>":["direct","value-string|integer|null","target-name"]},["id","end-feed","string|integer|null,..."]:["id","terminate","string|integer|null,..."],["id","processes"]}]
EOF

compare-output $OUT $EXP && rm -f $OUT $EXP
