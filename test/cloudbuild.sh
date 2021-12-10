#!/bin/sh

set -u
export D=$1
R=$2

cd $R
rm -rf json_build

set -e
mkdir json_build
cd json_build
cmake ../json >/dev/null
make -j 2 install
cd ..
rm -rf json_build
set +e

for X in clang++ g++
do
    export X
    mkdir build
    (
        echo "Build $(cat _logs/commit.txt) on $D using $X at $(date '+%Y-%m-%d %H:%M')"
        (
            set -eu
            cd build
            CXX=$X cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release ..
            make -j 2
            make test
        )
        E=$?
        echo "Build and test exit code: $E"
        if [ "$E" != "0" ]; then
            cd build
            for E in *_expected.txt
            do
                B="$(basename $E _expected.txt)_out.txt"
                test -f "$B" || continue
                for F in $E $B
                do
                    echo $F
                    cat $F
                done
            done
        fi
    ) 2>&1 | tee -a "$R/_logs/$D-$X.log"
    rm -rf build
done
