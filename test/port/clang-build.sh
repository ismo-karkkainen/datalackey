#!/bin/sh

set -eu

clone () {
    git clone --branch master --depth 1 $1 >/dev/null
}

clone https://github.com/nlohmann/json.git
cd json
cmake . >/dev/null
make >/dev/null
sudo make install >/dev/null
cd ..
CXX=clang++ cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release $1
make -j 3
make test
