#!/bin/sh
set -eu
sudo zypper install -y cmake make gcc-c++ ruby >/dev/null
git clone --branch master --depth 1 https://github.com/nlohmann/json.git
cd json
cmake .
make
sudo make install
cd ..
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release $1
make -j 3
make test
