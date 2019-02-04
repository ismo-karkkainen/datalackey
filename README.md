# What is datalackey?

One day I figured I had written enough code that stores data just because
I have to store it someplace while it is being tossed around. Take a JSON
object, pass it to datalackey and it splits it by using key as data label
and the value as data. You can instruct datalackey to take labels and
contruct a JSON object, allowing re-naming of the labels to whatever the
program is expecting.

This is not in any way a database or anything like that. You could write
a couple scripts to gather file contents into single JSON object and pass
it to your program and to split a JSON object into separate files.

You are not supposed to run this manually, instead this is meant to be used
by other programs. Hence default install location to /usr/local/libexec.

See datalackeyshell repository located where this repository is, for some
simple tools that use datalackey.

# Building.

You need cmake and compiler for C++14. Currently clang on macOS, and clang
and gcc on Ubuntu have been tried. Assuming a build directory parallel to
datalackey directory, you can use:

    cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=DEBUG ../datalackey
    cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=RELEASE ../datalackey
    cmake -G Xcode

To specify the compiler, set for example:

    CXX=clang++
    CXX=g++

To build, assuming Unix Makefiles:

    make
    make test
    sudo make install


# Notes.

This is supposed to be able to handle other formats than JSON in the
future. Currently only JSON is supported. Hence options that have just one
allowed value at the moment. I preferred stating things explicitly rather
than relying on default values.

Scripts at the top level are for testing purposes. Hence they work good
enough for current purposes.

# Running.

You are not supposed to run this manually. Look for datalackeyshell
repository in the same place where you found this repository.

For the hard-core, passing an array is considered a command. A mapping
(JSON object) is considered to contain data to store. First see the output of

    datalackey --help

What you must always give is the data storage options. Either use --memory
for using main memory as storage, or --directory to specify the location
where data storage directory is placed. The former is fine if all you need
is temporary storage, the latter of course can handle persistent and large
data.

The commands are output via:

    datalackey -m --report commands | oneline-prettify-json

Either figure out the syntax (at the moment lacks things such as count
information for parts that can only be input once) or see note about
datalackeyshell above.

To pass data in, just pass a JSON object and it will be split up.

To figure out actual usage, I suggest you look at the test scripts. Also,
datalackeyshell.

Datalackey outputs each reply to command (an array) or requested data
(JSON object) in single line. Hence the programs that communicate with
datalackey can expect each line to contain something fully recognizable as
JSON.
