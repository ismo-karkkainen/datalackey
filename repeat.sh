#!/bin/sh

if [ $# -lt 2 ]; then
    echo "Usage: $(basename $0) count command..."
    echo "  Repeats command... count times."
    exit 1
fi

COUNT=$1
shift
while test $COUNT -gt 0
do
    $*
    COUNT=$(expr $COUNT - 1)
    echo "$COUNT repeats remaining."
done
