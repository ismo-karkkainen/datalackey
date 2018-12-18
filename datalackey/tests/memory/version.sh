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
$DL -m -i stdin JSON -o stdout JSON | oneline-keysort-json > $OUT

VER=$(cat $VH | grep "Version " | sed 's/;//' | awk '{ printf("%s", $NF) }')
INT=$(cat $VH | grep "Interface " | sed 's/;//' | awk '{ printf("%s", $NF) }')

cat > $EXP <<EOF
[1,"version","",{"commands":{"delete":["id","delete","string,..."],"end-feed":["id","end-feed","string|integer|null,..."],"feed":{"<direct>":["direct","value-string|integer|null","target-name"],"<feed-subcommand>":["<input>","<direct>"],"<feed-subcommands>":[["<feed-subcommand>","<feed-subcommands>"],["<feed-subcommand>"]],"<feed>":["id","feed","string-id","<feed-subcommands>"],"<input>":["input","source-label","target-name"]},"get":["id","get","string,..."],"no-op":["id","no-op"],"rename":["id","rename","string,string,..."],"run":{"<change-directory>":["change-directory","directory"],"<direct>":["direct","value-string|integer|null","target-name"],"<end-feed>":["end-feed"],"<env-clear>":["env-clear"],"<env>":["env","variable-name","value-string|integer|null"],"<in>":["in","JSON","stdin"],"<input>":["input","source-label","target-name"],"<notify>":["notify","data|process"],"<out>":["out","JSON|bytes","stdout|stderr"],"<output-postfix>":["output-postfix","unmapped-result-name-postfix"],"<output-prefix>":["output-prefix","prefix-for-unmapped-result-name"],"<output>":["output","result-name","label|null"],"<program>":["program","executable"],"<run-subcommand>":["<input>","<direct>","<env>","<env-clear>","<output>","<output-prefix>","<output-postfix>","<in>","<out>","<notify>","<end-feed>","<change-directory>","<program>"],"<run-subcommands>":[["<run-subcommand>","<run-subcommands>"],["<run-subcommand>"]],"<run>":["id","run","<run-subcommands>"]},"storage-info":["id","storage-info"],"terminate":["id","terminate","string|integer|null,..."],"version":["id","version"]},"datalackey":$VER,"interface":$INT}]
[1,"done",""]
EOF

compare-output $OUT $EXP && rm -f $OUT $EXP
