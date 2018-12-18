#!/bin/bash

if [ $# -ne 2 ]; then
    echo "Usage: $(basename $0) datalackey-executable case"
    exit 100
fi
B=$(basename $0 .sh)
C=$1
DL=$2
OUT="${B}_${C}_out.txt"
EXP="${B}_${C}_expected.txt"

UNKNOWN="sdofyxvcbkuh"

(
case $C in
identifier_missing)
    echo '[]'
    ;;
identifier_invalid_float)
    echo '[0.5]'
    ;;
identifier_invalid_type)
    echo '[{"invalid":"identifier"}]'
    ;;
command_missing)
    echo '["id"]'
    ;;
command_not_string)
    echo '["id",1]'
    ;;
command_unknown)
    echo '["id","'$UNKNOWN'"]'
    ;;
argument_not_integer)
    echo '["id","run",2.5]'
    ;;
argument_invalid)
    echo '["id","run",{"invalid":"argument"}]'
    ;;
data_identifier_not_string)
    echo '{"foo'
    echo 'bar":2}'
    ;;
data_identifier_error_format)
    echo '{1a7:2}'
    ;;
*)
    echo
    ;;
esac
) | $DL -m -i stdin JSON -o stdout JSON > "$OUT"

(
case $C in
identifier_missing)
    echo '[null,"error","identifier","missing"]'
    ;;
identifier_invalid_*)
    echo '[null,"error","identifier","invalid"]'
    ;;
command_missing)
    echo '["id","error","command","missing"]'
    echo '["id","done",""]'
    ;;
command_not_string)
    echo '["id","error","command","not-string"]'
    echo '["id","done",""]'
    ;;
command_unknown)
    echo '["id","error","command","unknown","'$UNKNOWN'"]'
    echo '["id","done",""]'
    ;;
argument_not_integer)
    echo '["id","run","error","argument","not-integer"]'
    echo '["id","done",""]'
    ;;
argument_invalid)
    echo '["id","run","error","argument","invalid"]'
    echo '["id","done",""]'
    ;;
data_identifier_*)
    echo '[null,"error","format"]'
    ;;
*)
    echo "Invalid $0 argument: $C"
    ;;
esac
) > "$EXP"

compare-output "$OUT" "$EXP" && rm -f "$OUT" "$EXP"
