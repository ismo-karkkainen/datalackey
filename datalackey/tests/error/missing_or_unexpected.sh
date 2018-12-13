#!/bin/bash

if [ $# -ne 2 ]; then
    echo "Usage: $(basename $0) case datalackey-executable"
    exit 100
fi
B=$(basename $0 .sh)
C=$1
DL=$2
OUT="${B}_${C}_out.txt"
EXP="${B}_${C}_expected.txt"

(
case $C in
delete)
    echo '["id","delete"]'
    ;;
end-feed)
    echo '["id","end-feed"]'
    ;;
feed)
    echo '["id","feed"]'
    ;;
get)
    echo '["id","get"]'
    ;;
rename)
    echo '["id","rename"]'
    echo '["id","rename","pairless"]'
    ;;
run)
    echo '["id","run"]'
    ;;
storage-info)
    echo '["id","storage-info","unexpected"]'
    ;;
terminate)
    echo '["id","terminate"]'
    ;;
version)
    echo '["id","version","unexpected"]'
    ;;
*)
    echo
    ;;
esac
) | $DL -m -i stdin JSON -o stdout JSON > $OUT

(
case $C in
delete)
    echo '["id","error","missing","id","delete"]'
    ;;
end-feed)
    echo '["id","error","missing","id","end-feed"]'
    ;;
feed)
    echo '["id","error","missing","id","feed"]'
    ;;
get)
    echo '["id","error","missing","id","get"]'
    ;;
rename)
    echo '["id","error","missing","id","rename"]'
    echo '["id","error","pairless","id","rename","pairless"]'
    ;;
run)
    echo '["id","error","missing","id","run"]'
    ;;
storage-info)
    echo '["id","error","unexpected","id","storage-info","unexpected"]'
    ;;
terminate)
    echo '["id","error","missing","id","terminate"]'
    ;;
version)
    echo '["id","error","unexpected","id","version","unexpected"]'
    ;;
*)
    echo "Invalid test script argument: $C"
    ;;
esac
) > $EXP

compare-output "$OUT" "$EXP" && rm -f "$OUT" "$EXP"
