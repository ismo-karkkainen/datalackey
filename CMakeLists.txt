cmake_minimum_required(VERSION 3.5)
project(datalackey CXX)

if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    set(GenericOptions -std=c++14 -Wall)
    set(ReleaseOptions -O2)
    set(AlwaysSmallOptions -Os)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(GenericOptions -std=c++14 -Wall)
    set(ReleaseOptions -O2)
    set(AlwaysSmallOptions -Os)
#elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
#  # using Intel C++
#elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
#  # using Visual Studio C++
else()
    set(GenericOptions "")
    set(ReleaseOptions "")
endif()

if (UNIX AND NOT APPLE)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
endif()

add_compile_options( ${GenericOptions} )
if ("${CMAKE_BUILD_TYPE}" STREQUAL "RELEASE")
    add_compile_options( ${ReleaseOptions} )
endif()

include_directories( /usr/local/include/ )
include_directories( datalackey )
link_directories( /usr/local/lib/ )

enable_testing()

set(Sources datalackey/SimpleValue.cpp)
list(APPEND Sources datalackey/StringValue.cpp)
list(APPEND Sources datalackey/NumberValue.cpp)
list(APPEND Sources datalackey/NullValue.cpp)
list(APPEND Sources datalackey/Time.cpp)
list(APPEND Sources datalackey/File.cpp)
list(APPEND Sources datalackey/RawData.cpp)
list(APPEND Sources datalackey/DataReader.cpp)
list(APPEND Sources datalackey/DataOwner.cpp)
list(APPEND Sources datalackey/MemoryReader.cpp)
list(APPEND Sources datalackey/RawDataOwner.cpp)
list(APPEND Sources datalackey/MemoryOwner.cpp)
list(APPEND Sources datalackey/FileOwner.cpp)
list(APPEND Sources datalackey/FileReader.cpp)
list(APPEND Sources datalackey/DataGroup.cpp)
list(APPEND Sources datalackey/Storage.cpp)
list(APPEND Sources datalackey/MemoryStorage.cpp)
list(APPEND Sources datalackey/DirectoryStorage.cpp)
list(APPEND Sources datalackey/StorageDataSink.cpp)
list(APPEND Sources datalackey/StorageDataSinkJSON.cpp)

list(APPEND Sources datalackey/Literal.cpp)
list(APPEND Sources datalackey/ValueReference.cpp)
list(APPEND Sources datalackey/Encoder.cpp)
list(APPEND Sources datalackey/NullEncoder.cpp)
list(APPEND Sources datalackey/OutputChannel.cpp)
list(APPEND Sources datalackey/NullOutput.cpp)
list(APPEND Sources datalackey/OutputItem.cpp)
list(APPEND Sources datalackey/Output.cpp)
list(APPEND Sources datalackey/OutputCollection.cpp)
list(APPEND Sources datalackey/Message.cpp)
list(APPEND Sources datalackey/Messages.cpp)
list(APPEND Sources datalackey/ProcessMessages.cpp)
list(APPEND Sources datalackey/MessageReporter.cpp)
list(APPEND Sources datalackey/FileDescriptorOutput.cpp)
list(APPEND Sources datalackey/MessageHandler.cpp)
list(APPEND Sources datalackey/StorageDataSink.cpp)
list(APPEND Sources datalackey/InputChannel.cpp)
list(APPEND Sources datalackey/InputScanner.cpp)
list(APPEND Sources datalackey/InputScannerRawMessage.cpp)
list(APPEND Sources datalackey/InputScannerDiscard.cpp)
list(APPEND Sources datalackey/FileDescriptorInput.cpp)
list(APPEND Sources datalackey/StringValueMapper.cpp)
list(APPEND Sources datalackey/StringValueMapperSimple.cpp)

list(APPEND Sources datalackey/JSONEncoder.cpp)
list(APPEND Sources datalackey/StorageDataSinkJSON.cpp)
list(APPEND Sources datalackey/InputScannerJSON.cpp)
list(APPEND Sources datalackey/MessageRawJSON.cpp)

list(APPEND Sources datalackey/Command.cpp)
list(APPEND Sources datalackey/CommandHandler.cpp)
list(APPEND Sources datalackey/ListCommand.cpp)
list(APPEND Sources datalackey/StorageInfoCommand.cpp)
list(APPEND Sources datalackey/GetCommand.cpp)
list(APPEND Sources datalackey/DeleteCommand.cpp)
list(APPEND Sources datalackey/RenameCommand.cpp)
list(APPEND Sources datalackey/VersionCommand.cpp)
list(APPEND Sources datalackey/NoOperationCommand.cpp)

list(APPEND Sources datalackey/ProcessInput.cpp)
list(APPEND Sources datalackey/Process.cpp)
list(APPEND Sources datalackey/Processes.cpp)
list(APPEND Sources datalackey/LocalProcess.cpp)
list(APPEND Sources datalackey/CommandHandlerJSON.cpp)
list(APPEND Sources datalackey/Factories.cpp)
list(APPEND Sources datalackey/LocalProcesses.cpp)
list(APPEND Sources datalackey/ProcessesCommand.cpp)
list(APPEND Sources datalackey/RunCommand.cpp)
list(APPEND Sources datalackey/FeedCommand.cpp)
list(APPEND Sources datalackey/EndFeedCommand.cpp)
list(APPEND Sources datalackey/TerminateCommand.cpp)

list(APPEND Sources datalackey/Options.cpp)
list(APPEND Sources datalackey/datalackey.cpp)

add_executable( datalackey ${Sources} )
if (UNIX AND NOT APPLE)
    target_link_libraries(datalackey Threads::Threads)
endif()

list(APPEND AlwaysSmall ${Sources})
list(REMOVE_ITEM AlwaysSmall datalackey/RawData.cpp)
#list(REMOVE_ITEM AlwaysSmall datalackey/FileOwner.cpp) # File I/O
list(REMOVE_ITEM AlwaysSmall datalackey/DataGroup.cpp)
#list(REMOVE_ITEM AlwaysSmall datalackey/Storage.cpp) # Base
#list(REMOVE_ITEM AlwaysSmall datalackey/StorageDataSink.cpp) # Base
#list(REMOVE_ITEM AlwaysSmall datalackey/Literal.cpp) # Simple
#list(REMOVE_ITEM AlwaysSmall datalackey/ValueReference.cpp) # Simple
#list(REMOVE_ITEM AlwaysSmall datalackey/Encoder.cpp) # Base
#list(REMOVE_ITEM AlwaysSmall datalackey/NullEncoder.cpp) # Simple
#list(REMOVE_ITEM AlwaysSmall datalackey/OutputChannel.cpp) # Base
#list(REMOVE_ITEM AlwaysSmall datalackey/NullOutput.cpp) # Simple
#list(REMOVE_ITEM AlwaysSmall datalackey/OutputItem.cpp) # Base
list(REMOVE_ITEM AlwaysSmall datalackey/Output.cpp)
#list(REMOVE_ITEM AlwaysSmall datalackey/Messages.cpp) # Simple
#list(REMOVE_ITEM AlwaysSmall datalackey/FileDescriptorOutput.cpp) # File I/O
#list(REMOVE_ITEM AlwaysSmall datalackey/MessageHandler.cpp) # Base
#list(REMOVE_ITEM AlwaysSmall datalackey/StorageDataSink.cpp) # Base
#list(REMOVE_ITEM AlwaysSmall datalackey/InputChannel.cpp) # Base
list(REMOVE_ITEM AlwaysSmall datalackey/InputScanner.cpp)
#list(REMOVE_ITEM AlwaysSmall datalackey/InputScannerRawMessage.cpp) # Simple
#list(REMOVE_ITEM AlwaysSmall datalackey/InputScannerDiscard.cpp) # Simple
#list(REMOVE_ITEM AlwaysSmall datalackey/FileDescriptorInput.cpp) # File I/O
#list(REMOVE_ITEM AlwaysSmall datalackey/StringValueMapper.cpp) # Base
#list(REMOVE_ITEM AlwaysSmall datalackey/StringValueMapperSimple.cpp) # Simple
list(REMOVE_ITEM AlwaysSmall datalackey/JSONEncoder.cpp)
list(REMOVE_ITEM AlwaysSmall datalackey/StorageDataSinkJSON.cpp)
list(REMOVE_ITEM AlwaysSmall datalackey/InputScannerJSON.cpp)
list(REMOVE_ITEM AlwaysSmall datalackey/MessageRawJSON.cpp)
#list(REMOVE_ITEM AlwaysSmall datalackey/Command.cpp) # Base
#list(REMOVE_ITEM AlwaysSmall datalackey/CommandHandler.cpp) # Base
#list(REMOVE_ITEM AlwaysSmall datalackey/ListCommand.cpp) # Simple
#list(REMOVE_ITEM AlwaysSmall datalackey/StorageInfoCommand.cpp) # Simple
#list(REMOVE_ITEM AlwaysSmall datalackey/GetCommand.cpp) # Simple
#list(REMOVE_ITEM AlwaysSmall datalackey/DeleteCommand.cpp) # Simple
#list(REMOVE_ITEM AlwaysSmall datalackey/RenameCommand.cpp) # Simple
#list(REMOVE_ITEM AlwaysSmall datalackey/VersionCommand.cpp) # Simple
#list(REMOVE_ITEM AlwaysSmall datalackey/NoOperationCommand.cpp) # Simple
#list(REMOVE_ITEM AlwaysSmall datalackey/ProcessInput.cpp) # Simple
#list(REMOVE_ITEM AlwaysSmall datalackey/Process.cpp) # Base
#list(REMOVE_ITEM AlwaysSmall datalackey/Processes.cpp) # Base
list(REMOVE_ITEM AlwaysSmall datalackey/LocalProcess.cpp)
list(REMOVE_ITEM AlwaysSmall datalackey/CommandHandlerJSON.cpp)
#list(REMOVE_ITEM AlwaysSmall datalackey/Factories.cpp) # Simple
list(REMOVE_ITEM AlwaysSmall datalackey/LocalProcesses.cpp)
#message(${AlwaysSmall})

#set_source_files_properties(${AlwaysSmall} PROPERTIES COMPILE_FLAGS ${AlwaysSmallOptions})

install(TARGETS datalackey RUNTIME DESTINATION libexec)

#
# Tests.
#
function(add_exe SUBDIR PREFIX)
    if(ARGC EQUAL 3)
        set(FULL ${PREFIX}${ARGV2})
        set(BASE_NAME ${ARGV2})
    else()
        set(FULL ${PREFIX})
        set(BASE_NAME ${PREFIX})
    endif()
    add_executable(${FULL} IMPORTED)
    set_property(TARGET ${FULL} PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/datalackey/tests/${SUBDIR}/${BASE_NAME})
endfunction()

function(new_test TEST_NAME EXE)
    add_test(NAME ${TEST_NAME} COMMAND ${EXE} $<TARGET_FILE:datalackey>)
    set_property(TEST ${TEST_NAME} PROPERTY ENVIRONMENT "PATH=${CMAKE_CURRENT_LIST_DIR}:$ENV{PATH}")
endfunction()

function(new_test_arg TEST_NAME EXE ARG)
    add_test(NAME ${TEST_NAME} COMMAND ${EXE} ${ARG} $<TARGET_FILE:datalackey>)
    set_property(TEST ${TEST_NAME} PROPERTY ENVIRONMENT "PATH=${CMAKE_CURRENT_LIST_DIR}:$ENV{PATH}")
endfunction()

# Exit code tests.
#
add_exe(memory exit.sh)
new_test_arg(exit0 exit.sh 0)
new_test_arg(exit9 exit.sh 9)

# Signal tests.
#
add_exe(memory signal.sh)
new_test_arg(signal6 signal.sh 6)
new_test_arg(signal9 signal.sh 9)
add_exe(memory stop.sh)
new_test(stopcont stop.sh)

# Simple commands
#
add_exe(memory noop.sh)
new_test(noop noop.sh)
add_exe(memory version.sh)
new_test_arg(version version.sh ${CMAKE_CURRENT_LIST_DIR}/datalackey/Version.hpp)

# Processes
#
add_exe(memory processes.sh)
new_test(processes processes.sh)
add_exe(memory terminate.sh)
new_test(terminate terminate.sh)
add_exe(memory notify_data_err.sh)
new_test(notify_data_err notify_data_err.sh)
add_exe(memory change-directory.sh)
new_test(change-directory change-directory.sh)
add_exe(memory abs-change-directory.sh)
new_test(abs-change-directory abs-change-directory.sh)
add_exe(memory bad-change-directory.sh)
new_test(bad-change-directory bad-change-directory.sh)
add_exe(memory bad-program.sh)
new_test(bad-program bad-program.sh)

# Data handling
#
add_exe(memory set_delete.sh)
new_test(set_delete set_delete.sh)
add_exe(memory set_multiple.sh)
new_test(set_multiple set_multiple.sh)
add_exe(memory rename.sh)
new_test(rename rename.sh)

# Message output from process tests.
#
add_exe(memory raw.sh)
new_test(raw_abc raw.sh)
add_exe(memory stderr_raw_stdout.sh)
new_test(raw_stderr_abc stderr_raw_stdout.sh)
add_exe(memory stderr_raw_stderr.sh)
new_test(raw_stderr2_abc stderr_raw_stderr.sh)
add_exe(memory output.sh)
new_test(output_get output.sh)
add_exe(memory output_etc.sh)
new_test(output_etc output_etc.sh)
add_exe(memory null_terminate.sh)
new_test(null_terminate null_terminate.sh)
add_exe(memory feed_end.sh)
new_test(feed_end feed_end.sh)

# Bad label output
#
add_exe(memory bad_label_output.sh)
new_test_arg(label_null bad_label_output.sh null)
new_test_arg(label_number bad_label_output.sh 4)
new_test_arg(label_chars bad_label_output.sh bad)
new_test_arg(label_array bad_label_output.sh '[\ 4\ ]')
new_test_arg(label_object bad_label_output.sh '{\ "a":\ 4\ }')

# Controller runs a program
#
add_exe(memory ctrl_notify_none.sh)
new_test(ctrl_notify_none ctrl_notify_none.sh)
add_exe(memory ctrl_notify_data.sh)
new_test(ctrl_notify_data ctrl_notify_data.sh)
add_exe(memory input_eof.sh)
new_test(input_eof input_eof.sh)
add_exe(memory notify_data_child.sh)
new_test(notify_data_child notify_data_child.sh)

# Directory storage tests
#
add_exe(directory d_ output.sh)
new_test(d_output_get d_output.sh)
add_exe(directory d_ output_etc.sh)
new_test(d_output_etc d_output_etc.sh)
add_exe(directory d_ rename.sh)
new_test(d_rename d_rename.sh)
add_exe(directory d_ set_delete.sh)
new_test(d_set_delete d_set_delete.sh)
add_exe(directory d_ feed_end.sh)
new_test(d_feed_end d_feed_end.sh)
add_exe(directory d_ ctrl_notify_none.sh)
new_test(d_ctrl_notify_none d_ctrl_notify_none.sh)
add_exe(directory d_ relative_dir.sh)
new_test(d_relative_dir d_relative_dir.sh)
add_exe(directory d_ bad_dir.sh)
new_test(d_bad_dir d_bad_dir.sh)
add_exe(directory d_ set_reload.sh)
new_test(d_set_reload d_set_reload.sh)

# Missing/unexpected arguments and errors.
#
add_exe(error missing_or_unexpected.sh)
new_test_arg(delete_missing missing_or_unexpected.sh delete)
new_test_arg(end_feed_missing missing_or_unexpected.sh end-feed)
new_test_arg(feed_missing missing_or_unexpected.sh feed)
new_test_arg(get_missing missing_or_unexpected.sh get)
new_test_arg(list_unexpected missing_or_unexpected.sh list)
new_test_arg(processes_unexpected missing_or_unexpected.sh processes)
new_test_arg(rename_missing missing_or_unexpected.sh rename)
new_test_arg(run_missing missing_or_unexpected.sh run)
new_test_arg(storage_info_unexpected missing_or_unexpected.sh storage-info)
new_test_arg(terminate_missing missing_or_unexpected.sh terminate)
new_test_arg(version_unexpected missing_or_unexpected.sh version)

# Various errors.
#
add_exe(error error.sh)
new_test_arg(identifier_missing error.sh identifier_missing)
new_test_arg(identifier_invalid_float error.sh identifier_invalid_float)
new_test_arg(identifier_invalid_type error.sh identifier_invalid_type)
new_test_arg(command_missing error.sh command_missing)
new_test_arg(command_not_string error.sh command_not_string)
new_test_arg(command_unknown error.sh command_unknown)
new_test_arg(argument_not_integer error.sh argument_not_integer)
new_test_arg(argument_invalid error.sh argument_invalid)
new_test_arg(data_identifier_not_string error.sh data_identifier_not_string)
new_test_arg(data_identifier_error_format error.sh data_identifier_error_format)
add_exe(error feed.sh)
new_test(feed_errors feed.sh)
add_exe(error run.sh)
new_test(run_errors run.sh)
