#!/bin/bash

if [ $# -ne 2 ]; then
    echo "Usage: $(basename $0) report datalackey-executable"
    exit 100
fi
B=$(basename $0 .sh)
REPORT=$1
DL=$2
OUT="${B}_out.txt"
EXP="${B}_expected.txt"

$DL -m --report $REPORT -i stdin JSON -o stdout JSON |
oneline-keysort-json | sort -u > $OUT

if [ "$REPORT" = "messages" ]; then
cat > $EXP <<EOF
["id","?","error","argument","invalid"]
["id","?","error","argument","not-integer"]
["id","channel","reset"]
["id","delete","missing","..."]
["id","done",""]
["id","end-feed","missing","..."]
["id","end-feed","not-open","..."]
["id","error","command","missing","?"]
["id","error","command","not-string","?"]
["id","error","command","unknown","?"]
["id","error","format"]
["id","error","missing","..."]
["id","error","not-string","..."]
["id","error","not-string-null","..."]
["id","error","pairless","..."]
["id","error","unexpected","..."]
["id","error","unknown","..."]
["id","feed","error","?","argument","unknown"]
["id","feed","error","?","duplicate","?"]
["id","feed","error","closed"]
["id","feed","error","missing","..."]
["id","feed","error","not-found"]
["id","get","","mapping label-to-value"]
["id","get","failed","..."]
["id","get","missing","..."]
["id","no-op",""]
["id","rename","missing","..."]
["id","run","continued"]
["id","run","error","?","argument","unknown"]
["id","run","error","?","duplicate","?"]
["id","run","error","change-directory","?","?"]
["id","run","error","env","argument","duplicate","?"]
["id","run","error","env","argument","invalid","?"]
["id","run","error","exception"]
["id","run","error","format"]
["id","run","error","identifier","in-use"]
["id","run","error","in","missing"]
["id","run","error","in","multiple"]
["id","run","error","input","failed"]
["id","run","error","missing","..."]
["id","run","error","no-memory"]
["id","run","error","no-processes"]
["id","run","error","no-thread"]
["id","run","error","notify","no-input"]
["id","run","error","out","duplicate","?"]
["id","run","error","out","missing"]
["id","run","error","output","duplicate","?"]
["id","run","error","pipe"]
["id","run","error","program","?","?"]
["id","run","error","program","?"]
["id","run","exit",0]
["id","run","finished"]
["id","run","input","closed"]
["id","run","running",0]
["id","run","signal",0]
["id","run","stopped",0]
["id","run","terminated"]
["id","storage-info","","mapping label-to-mapping-format-to-size"]
["id","terminate","missing","..."]
["id","version","","mapping"]
[null,"data","deleted","string",0]
[null,"data","error","string",0]
[null,"data","stored","string",0]
[null,"error","format"]
[null,"error","identifier","invalid"]
[null,"error","identifier","missing"]
[null,"process","ended","id",0]
[null,"process","started","id",0]
EOF
else
cat > $EXP <<EOF
{"delete":["id","delete","string,..."],"end-feed":["id","end-feed","string|integer|null,..."],"feed":{"<direct>":["direct","value-string|integer|null","target-name"],"<feed-subcommand>":["<input>","<direct>"],"<feed-subcommands>":[["<feed-subcommand>","<feed-subcommands>"],["<feed-subcommand>"]],"<feed>":["id","feed","string-id","<feed-subcommands>"],"<input>":["input","source-label","target-name"]},"get":["id","get","string,..."],"no-op":["id","no-op"],"rename":["id","rename","string,string,..."],"run":{"<change-directory>":["change-directory","directory"],"<direct>":["direct","value-string|integer|null","target-name"],"<end-feed>":["end-feed"],"<env-clear>":["env-clear"],"<env>":["env","variable-name","value-string|integer|null"],"<in>":["in","JSON","stdin"],"<input>":["input","source-label","target-name"],"<notify>":["notify","data|process"],"<out>":["out","JSON|bytes","stdout|stderr"],"<output-postfix>":["output-postfix","unmapped-result-name-postfix"],"<output-prefix>":["output-prefix","prefix-for-unmapped-result-name"],"<output>":["output","result-name","label|null"],"<program>":["program","executable"],"<run-subcommand>":["<input>","<direct>","<env>","<env-clear>","<output>","<output-prefix>","<output-postfix>","<in>","<out>","<notify>","<end-feed>","<change-directory>","<program>"],"<run-subcommands>":[["<run-subcommand>","<run-subcommands>"],["<run-subcommand>"]],"<run>":["id","run","<run-subcommands>"]},"storage-info":["id","storage-info"],"terminate":["id","terminate","string|integer|null,..."],"version":["id","version"]}
EOF
fi

compare-output $OUT $EXP && rm -rf $OUT $EXP
