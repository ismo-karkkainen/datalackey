#!/bin/sh
set -eu
sudo apt-get install -y -q cmake build-essential ruby >/dev/null
git clone --branch master --depth 1 https://github.com/nlohmann/json.git >/dev/null
cd json
cmake . >/dev/null
make >/dev/null
sudo make install >/dev/null
cd ..
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release $1
make -j 3
make test
