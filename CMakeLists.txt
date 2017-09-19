cmake_minimum_required (VERSION 3.5)
project (datalackey)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

include_directories( /usr/local/include/ )
include_directories( datalackey )
link_directories( /usr/local/lib/ )
link_libraries( boost_system boost_filesystem )

enable_testing()
set(Sources datalackey/main.cpp)

set(StoreSources datalackey/RawData.cpp datalackey/Storage.cpp)
list(APPEND StoreSources datalackey/Converter.cpp datalackey/MemoryStorage.cpp)
list(APPEND StoreSources datalackey/StorageDataSink.cpp datalackey/StorageDataSinkJSON.cpp)

set(IOSources datalackey/Literal.cpp datalackey/ValueReference.cpp)
list(APPEND IOSources datalackey/Encoder.cpp)
list(APPEND IOSources datalackey/OutputChannel.cpp datalackey/Output.cpp)
list(APPEND IOSources datalackey/StdOut.cpp datalackey/StdErr.cpp)
list(APPEND IOSources datalackey/MessageHandler.cpp datalackey/MessagePassThrough.cpp)
list(APPEND IOSources datalackey/StorageDataSink.cpp)
list(APPEND IOSources datalackey/InputChannel.cpp datalackey/InputScanner.cpp)
list(APPEND IOSources datalackey/StdIn.cpp)

set(JSONIO datalackey/JSONEncoder.cpp datalackey/StorageDataSinkJSON.cpp)
list(APPEND JSONIO datalackey/InputScannerJSON.cpp)

set(Commands datalackey/Command.cpp datalackey/CommandHandler.cpp)
list(APPEND Commands datalackey/ListCommand.cpp)

set(JSONCommands datalackey/CommandHandlerJSON.cpp)

set(CommandLine datalackey/Options.cpp)

#add_executable( test_FileLister datalackey/Encoder.cpp datalackey/JSONEncoder.cpp datalackey/OutputChannel.cpp datalackey/Output.cpp datalackey/Literal.cpp datalackey/StdOut.cpp datalackey/ValueReference.cpp datalackey/Literal.cpp datalackey/FileLister.cpp datalackey/tests/test_FileLister.cpp )

#add_executable(test_FileLister.sh IMPORTED)
#set_property(TARGET test_FileLister.sh  PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/datalackey/tests/test_FileLister.sh)
#add_test(NAME FileLister_List COMMAND test_FileLister.sh $<TARGET_FILE:test_FileLister> )

add_executable( test_MessagePassThrough ${StoreSources} ${IOSources} ${JSONIO} datalackey/tests/test_MessagePassThrough.cpp )


add_executable( test_command ${StoreSources} ${IOSources} ${JSONIO} ${Commands} ${JSONCommands} ${CommandLine} datalackey/tests/test_command.cpp )

# Add various components to source files list provided the list has not been
# given from outside.


#add_executable (datalackey ${Sources} ${IOSources})

