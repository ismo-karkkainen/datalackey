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
list)
    echo '["id","list","unexpected"]'
    ;;
processes)
    echo '["id","processes","unexpected"]'
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
    echo '["id","delete","error","argument","missing"]'
    ;;
end-feed)
    echo '["id","end-feed","error","argument","missing"]'
    ;;
feed)
    echo '["id","feed","error","argument","missing"]'
    ;;
get)
    echo '["id","get","error","argument","missing"]'
    ;;
list)
    echo '["id","list","error","argument","unexpected"]'
    ;;
processes)
    echo '["id","processes","error","argument","unexpected"]'
    ;;
rename)
    echo '["id","rename","error","argument","missing"]'
    echo '["id","rename","error","argument","pairless"]'
    ;;
run)
    echo '["id","run","error","argument","missing"]'
    ;;
storage-info)
    echo '["id","storage-info","error","argument","unexpected"]'
    ;;
terminate)
    echo '["id","terminate","error","argument","missing"]'
    ;;
version)
    echo '["id","version","error","argument","unexpected"]'
    ;;
*)
    echo "Invalid test script argument: $C"
    ;;
esac
) > $EXP

compare-output "$OUT" "$EXP" && rm -f "$OUT" "$EXP"
