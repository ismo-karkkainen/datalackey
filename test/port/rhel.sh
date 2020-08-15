#!/bin/sh
set -eu
sudo yum install -y -q cmake make gcc-c++ ruby
git clone --branch master --depth 1 https://github.com/nlohmann/json.git
cd json
cmake .
make
sudo make install
cd ..
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=DEBUG $1
make -j 3
make test
