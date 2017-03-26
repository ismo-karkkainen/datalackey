cmake_minimum_required (VERSION 3.5)
project (datalackey)

include_directories( /usr/local/include/ )
include_directories( datalackey )
link_directories( /usr/local/lib/ )
link_libraries( boost_system boost_filesystem )

enable_testing()
set (SOURCES datalackey/main.cpp datalackey/FileLister.cpp)


# Add various components to source files list provided the list has not been
# given from outside.


#add_executable (datalackey ${SOURCES})

add_executable( test_FileLister datalackey/FileLister.cpp datalackey/tests/test_FileLister.cpp )

add_test( FileLister_List test_FileLister )
