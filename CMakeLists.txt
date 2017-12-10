cmake_minimum_required (VERSION 3.5)
project (datalackey)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

include_directories( /usr/local/include/ )
include_directories( datalackey )
link_directories( /usr/local/lib/ )
link_libraries( boost_system boost_filesystem )

enable_testing()
set(Sources datalackey/datalackey.cpp)

set(StoreSources datalackey/SimpleValue.cpp)
list(APPEND StoreSources datalackey/StringValue.cpp)
list(APPEND StoreSources datalackey/NumberValue.cpp)
list(APPEND StoreSources datalackey/NullValue.cpp)
list(APPEND StoreSources datalackey/Time.cpp)
list(APPEND StoreSources datalackey/RawData.cpp datalackey/Storage.cpp)
list(APPEND StoreSources datalackey/Converter.cpp datalackey/MemoryStorage.cpp)
list(APPEND StoreSources datalackey/StorageDataSink.cpp datalackey/StorageDataSinkJSON.cpp)

set(IOSources datalackey/Literal.cpp)
list(APPEND IOSources datalackey/ValueReference.cpp)
list(APPEND IOSources datalackey/Encoder.cpp)
list(APPEND IOSources datalackey/OutputChannel.cpp)
list(APPEND IOSources datalackey/OutputItem.cpp)
list(APPEND IOSources datalackey/Output.cpp)
list(APPEND IOSources datalackey/Notifications.cpp)
list(APPEND IOSources datalackey/FileDescriptorOutput.cpp)
list(APPEND IOSources datalackey/MessageHandler.cpp)
list(APPEND IOSources datalackey/MessagePassThrough.cpp)
list(APPEND IOSources datalackey/StorageDataSink.cpp)
list(APPEND IOSources datalackey/InputChannel.cpp)
list(APPEND IOSources datalackey/InputScanner.cpp)
list(APPEND IOSources datalackey/InputScannerRawMessage.cpp)
list(APPEND IOSources datalackey/InputScannerDiscard.cpp)
list(APPEND IOSources datalackey/FileDescriptorInput.cpp)
list(APPEND IOSources datalackey/StringValueMapper.cpp)
list(APPEND IOSources datalackey/StringValueMapperSimple.cpp)

set(JSONIO datalackey/JSONEncoder.cpp)
list(APPEND JSONIO datalackey/StorageDataSinkJSON.cpp)
list(APPEND JSONIO datalackey/InputScannerJSON.cpp)
list(APPEND JSONIO datalackey/MessageRawJSON.cpp)

set(Commands datalackey/Command.cpp)
list(APPEND Commands datalackey/CommandHandler.cpp)
list(APPEND Commands datalackey/ListCommand.cpp)
list(APPEND Commands datalackey/GetCommand.cpp)
list(APPEND Commands datalackey/DeleteCommand.cpp)
list(APPEND Commands datalackey/VersionCommand.cpp)
list(APPEND Commands datalackey/NoOperationCommand.cpp)

set(Processes datalackey/Process.cpp)
list(APPEND Processes datalackey/Processes.cpp)
list(APPEND Processes datalackey/LocalProcess.cpp)
list(APPEND Processes datalackey/LocalProcesses.cpp)
set(ProcessCommands datalackey/ProcessesCommand.cpp)
list(APPEND ProcessCommands datalackey/RunCommand.cpp)
list(APPEND ProcessCommands datalackey/TerminateCommand.cpp)

set(JSONCommands datalackey/CommandHandlerJSON.cpp)

set(CommandLine datalackey/Options.cpp)

add_executable( datalackey ${StoreSources} ${IOSources} ${JSONIO} ${Commands} ${JSONCommands} ${CommandLine} ${Processes} ${ProcessCommands} ${Sources} )

#
# Tests.
#

# Exit code tests.
#
add_executable(test_exit.sh IMPORTED)
set_property(TARGET test_exit.sh PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/datalackey/tests/test_exit.sh)
add_test(NAME exit0 COMMAND test_exit.sh 0 $<TARGET_FILE:datalackey> )
add_test(NAME exit9 COMMAND test_exit.sh 9 $<TARGET_FILE:datalackey> )

# Signal tests.
#
add_executable(test_signal.sh IMPORTED)
set_property(TARGET test_signal.sh PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/datalackey/tests/test_signal.sh)
add_test(NAME signal6 COMMAND test_signal.sh 6 $<TARGET_FILE:datalackey> )
add_test(NAME signal9 COMMAND test_signal.sh 9 $<TARGET_FILE:datalackey> )

# Message output from process tests.
#
add_executable(test_raw.sh IMPORTED)
set_property(TARGET test_raw.sh PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/datalackey/tests/test_raw.sh)
add_test(NAME raw_abc COMMAND test_raw.sh $<TARGET_FILE:datalackey> )
add_executable(test_stderr_raw_stdout.sh IMPORTED)
set_property(TARGET test_stderr_raw_stdout.sh PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/datalackey/tests/test_stderr_raw_stdout.sh)
add_test(NAME raw_stderr_abc COMMAND test_stderr_raw_stdout.sh $<TARGET_FILE:datalackey> )
add_executable(test_stderr_raw_stderr.sh IMPORTED)
set_property(TARGET test_stderr_raw_stderr.sh PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/datalackey/tests/test_stderr_raw_stderr.sh)
add_test(NAME raw_stderr2_abc COMMAND test_stderr_raw_stderr.sh $<TARGET_FILE:datalackey> )
add_executable(test_output.sh IMPORTED)
set_property(TARGET test_output.sh PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/datalackey/tests/test_output.sh)
add_test(NAME output_get COMMAND test_output.sh $<TARGET_FILE:datalackey> )

