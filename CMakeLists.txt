cmake_minimum_required (VERSION 3.5)
project (datalackey)

include_directories( /usr/local/include/ )
include_directories( datalackey )
link_directories( /usr/local/lib/ )
link_libraries( boost_system boost_filesystem )

enable_testing()
set(Sources datalackey/main.cpp datalackey/FileLister.cpp)
list(APPEND Sources datalackey/MessageOutput.cpp datalackey/StdOutErr.cpp)

add_executable( test_FileLister datalackey/MessageOutput.cpp datalackey/StdOutErr.cpp datalackey/FileLister.cpp datalackey/tests/test_FileLister.cpp )

add_executable(test_FileLister.sh IMPORTED)
set_property(TARGET test_FileLister.sh  PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/datalackey/tests/test_FileLister.sh)
add_test(NAME FileLister_List COMMAND test_FileLister.sh $<TARGET_FILE:test_FileLister> )

# Add various components to source files list provided the list has not been
# given from outside.


#add_executable (datalackey ${Sources})

