#!/bin/sh
set -eu
sudo yum install -y -q cmake make clang ruby
git clone --branch master --depth 1 https://github.com/nlohmann/json.git >/dev/null
cd json
cmake . >/dev/null
make >/dev/null
sudo make install >/dev/null
cd ..
CXX=clang++ cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release $1
make -j 3
make test
