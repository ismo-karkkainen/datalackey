cmake_minimum_required (VERSION 3.5)
project (datalackey)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

include_directories( /usr/local/include/ )
include_directories( datalackey )
link_directories( /usr/local/lib/ )
#link_libraries( boost_system boost_filesystem )

enable_testing()
set(Sources datalackey/datalackey.cpp)

set(StoreSources datalackey/SimpleValue.cpp)
list(APPEND StoreSources datalackey/StringValue.cpp)
list(APPEND StoreSources datalackey/NumberValue.cpp)
list(APPEND StoreSources datalackey/NullValue.cpp)
list(APPEND StoreSources datalackey/Time.cpp)
list(APPEND StoreSources datalackey/RawData.cpp)
list(APPEND StoreSources datalackey/DataOwner.cpp)
list(APPEND StoreSources datalackey/RawDataOwner.cpp)
list(APPEND StoreSources datalackey/MemoryOwner.cpp)
list(APPEND StoreSources datalackey/FileOwner.cpp)
list(APPEND StoreSources datalackey/DataGroup.cpp)
list(APPEND StoreSources datalackey/Storage.cpp)
list(APPEND StoreSources datalackey/MemoryStorage.cpp)
list(APPEND StoreSources datalackey/StorageDataSink.cpp)
list(APPEND StoreSources datalackey/StorageDataSinkJSON.cpp)

set(IOSources datalackey/Literal.cpp)
list(APPEND IOSources datalackey/ValueReference.cpp)
list(APPEND IOSources datalackey/Encoder.cpp)
list(APPEND IOSources datalackey/OutputChannel.cpp)
list(APPEND IOSources datalackey/OutputItem.cpp)
list(APPEND IOSources datalackey/Output.cpp)
list(APPEND IOSources datalackey/Notifications.cpp)
list(APPEND IOSources datalackey/FileDescriptorOutput.cpp)
list(APPEND IOSources datalackey/MessageHandler.cpp)
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

set(Processes datalackey/ProcessInput.cpp)
list(APPEND Processes datalackey/Process.cpp)
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

# Simple commands
#
add_executable(test_noop.sh IMPORTED)
set_property(TARGET test_noop.sh PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/datalackey/tests/test_noop.sh)
add_test(NAME noop COMMAND test_noop.sh $<TARGET_FILE:datalackey> )
add_executable(test_version.sh IMPORTED)
set_property(TARGET test_version.sh PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/datalackey/tests/test_version.sh)
add_test(NAME version COMMAND test_version.sh ${CMAKE_CURRENT_LIST_DIR}/datalackey/Version.hpp $<TARGET_FILE:datalackey> )

# Processes
#
add_executable(test_processes.sh IMPORTED)
set_property(TARGET test_processes.sh PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/datalackey/tests/test_processes.sh)
add_test(NAME processes COMMAND test_processes.sh $<TARGET_FILE:datalackey> )
add_executable(test_terminate.sh IMPORTED)
set_property(TARGET test_terminate.sh PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/datalackey/tests/test_terminate.sh)
add_test(NAME terminate COMMAND test_terminate.sh $<TARGET_FILE:datalackey> )

# Data handling
#
add_executable(test_set_delete.sh IMPORTED)
set_property(TARGET test_set_delete.sh PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/datalackey/tests/test_set_delete.sh)
add_test(NAME set_delete COMMAND test_set_delete.sh $<TARGET_FILE:datalackey> )

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
add_executable(test_output_etc.sh IMPORTED)
set_property(TARGET test_output_etc.sh PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/datalackey/tests/test_output_etc.sh)
add_test(NAME output_etc COMMAND test_output_etc.sh $<TARGET_FILE:datalackey> )
add_executable(test_null_terminate.sh IMPORTED)
set_property(TARGET test_null_terminate.sh PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/datalackey/tests/test_null_terminate.sh)
add_test(NAME null_terminate COMMAND test_null_terminate.sh $<TARGET_FILE:datalackey> )

# Bad label output
#
add_executable(test_bad_label_output.sh IMPORTED)
set_property(TARGET test_bad_label_output.sh PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/datalackey/tests/test_bad_label_output.sh)
add_test(NAME label_null COMMAND test_bad_label_output.sh null $<TARGET_FILE:datalackey> )
add_test(NAME label_number COMMAND test_bad_label_output.sh 4 $<TARGET_FILE:datalackey> )
add_test(NAME label_chars COMMAND test_bad_label_output.sh bad $<TARGET_FILE:datalackey> )
add_test(NAME label_array COMMAND test_bad_label_output.sh '[\ 4\ ]' $<TARGET_FILE:datalackey> )
add_test(NAME label_object COMMAND test_bad_label_output.sh '{\ "a":\ 4\ }' $<TARGET_FILE:datalackey> )

