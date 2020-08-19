# Datalackey

Datalackey stores data, runs programs, passes data to them, and stores data
coming from the programs. Each data input and output is a JSON object. Each
data output get split at top level so that each key is used as data label
and each value is the data itself.

You are not supposed to run datalackey manually, instead it is meant to be
used by other programs. Hence default install location is /usr/local/libexec.

See datalackeyshell repository located where this repository is, for some
simple tools that use datalackey. Including a pair of ruby scripts that
construct a JSON object out of file contents and split JSON object into
files. For simple cases they are fully adequate for managing the data.

When passing data to programs, you can map labels to keys you want, and
the key/value pairs are passed to the program in one JSON object. In the
order you specified in the command.

Datalackey is not intended to be used as a database. If you think of Redis
or Memcached, the use of datalackey is to run programs it starts, gather
key/value pairs into one JSON object, pass it to the program and then split
the results and store the key/value pairs. You could use datalackey as a
data store but whether you should is debatable.

# Writing programs for use with datalackey

Datalackey outputs each reply to command (an array) or requested data
(JSON object) in a single line. Hence the programs that communicate with
datalackey can expect each line to contain something fully recognizable as
JSON. Therefore reading input one line at a time is fine. Always flush the
output to ensure it is sent to datalackey.

Datalackey has separate threads for reading and writing to the child or
parent processes. Hence it can handle the case where a process outputs a
lot of data while not reading the input without locking up. This may
simplify implementation of programs that read input data and produce
output as the implementation can be straightforward read, process, write.

# Requirements

The https://github.com/nlohmann/json version 3.6 or newer is required.
You need cmake and a C++ compiler that supports C++14.

On macOS with Homebrew (https://docs.brew.sh):

    brew tap nlohmann/json
    brew install nlohmann-json
    brew install cmake

On Linux you may need to install git to clone a sufficiently new version of
json library. Otherwise standard packages should be ok.

    git clone --depth 1 https://github.com/nlohmann/json
    cd json && cmake . && make && sudo make install

# Building

You need cmake and compiler for C++14. Currently clang on macOS, and clang
and gcc on Ubuntu have been tried. Assuming a build directory parallel to
main datalackey directory, you can use:

    cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug ../datalackey
    cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release ../datalackey
    cmake -G Xcode -DCMAKE_BUILD_TYPE=Release ../datalackey

To specify the compiler explicitly, set for example:

    CXX=clang++
    CXX=g++

To build, test, and install, assuming Unix Makefiles:

    make -j 10
    make test
    sudo make install

# Notes

As the source code indicates in places, various BSD variants have been tried
but even though the source code compiles, tests fail.

This is supposed to be able to handle other formats than JSON in the
future. Currently only JSON is supported. Hence options that have just one
allowed value at the moment. I preferred stating things explicitly rather
than relying on default values.

Scripts at the top level are for testing purposes. Hence they work good
enough for current purposes in the current context.

# Running

You are not supposed to run datalackey manually. Look for datalackeyshell
repository in the same place where you found this repository.

For the hard-core, passing an array is considered a command. A JSON object
is considered to contain data to store. First see the output of

    datalackey --help

What you must always give is a data storage option. Either use --memory
for using main memory as storage, or --directory to specify the location
where data storage directory is placed. The former is fine if all you need
is temporary storage, the latter of course can handle persistent and large
data. data will be sored in ".datalackey" directory under the directory you
gave. The given directory must exist.

The commands you can use are output via:

    datalackey -m --report commands | oneline-prettify-json

Either figure out the syntax (at the moment lacks things such as count
information for parts that can only be input once) or see note about
datalackeyshell above.

To pass data in, just pass a JSON object and it will be split up. Each JSON
object is treated as atomic. Either all items are stored or none are. Hence
bad output from source program will not result in partial updates. In case of
invalid JSON sent to datalackey, a zero byte can be used to reset the input
so that proper JSON can be set after the zero byte. White-space outside
strings is ignored, and removed for stored data.

To figure out actual usage, I suggest you look at the test scripts. Also,
datalackeyshell.

# License

Copyright (C) 2019, 2020 Ismo Kärkkäinen

Licensed under Universal Permissive License. See License.txt.
