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
    echo '["id","done",""]'
    ;;
end-feed)
    echo '["id","error","missing","id","end-feed"]'
    echo '["id","done",""]'
    ;;
feed)
    echo '["id","error","missing","id","feed"]'
    echo '["id","done",""]'
    ;;
get)
    echo '["id","error","missing","id","get"]'
    echo '["id","done",""]'
    ;;
rename)
    echo '["id","error","missing","id","rename"]'
    echo '["id","done",""]'
    echo '["id","error","pairless","id","rename","pairless"]'
    echo '["id","done",""]'
    ;;
run)
    echo '["id","error","missing","id","run"]'
    echo '["id","done",""]'
    ;;
storage-info)
    echo '["id","error","unexpected","id","storage-info","unexpected"]'
    echo '["id","done",""]'
    ;;
terminate)
    echo '["id","error","missing","id","terminate"]'
    echo '["id","done",""]'
    ;;
version)
    echo '["id","error","unexpected","id","version","unexpected"]'
    echo '["id","done",""]'
    ;;
*)
    echo "Invalid test script argument: $C"
    ;;
esac
) > $EXP

compare-output "$OUT" "$EXP" && rm -f "$OUT" "$EXP"
