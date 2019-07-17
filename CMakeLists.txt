cmake_minimum_required(VERSION 3.10)
project(datalackey CXX)

if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    set(GenericOptions -Wall)
    set(ReleaseOptions -O2)
    set(AlwaysSmallOptions -Os)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(GenericOptions -Wall)
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

find_package(nlohmann_json 3.4.0 REQUIRED)
if (UNIX AND NOT APPLE)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
endif()

enable_testing()

set(Sources src/SimpleValue.cpp)
list(APPEND Sources src/StringValue.cpp)
list(APPEND Sources src/NumberValue.cpp)
list(APPEND Sources src/NullValue.cpp)
list(APPEND Sources src/Time.cpp)
list(APPEND Sources src/File.cpp)
list(APPEND Sources src/RawData.cpp)
list(APPEND Sources src/DataReader.cpp)
list(APPEND Sources src/DataOwner.cpp)
list(APPEND Sources src/MemoryReader.cpp)
list(APPEND Sources src/RawDataOwner.cpp)
list(APPEND Sources src/MemoryOwner.cpp)
list(APPEND Sources src/FileOwner.cpp)
list(APPEND Sources src/FileReader.cpp)
list(APPEND Sources src/DataGroup.cpp)
list(APPEND Sources src/Storage.cpp)
list(APPEND Sources src/MemoryStorage.cpp)
list(APPEND Sources src/DirectoryStorage.cpp)
list(APPEND Sources src/StorageDataSink.cpp)
list(APPEND Sources src/StorageDataSinkJSON.cpp)

list(APPEND Sources src/Literal.cpp)
list(APPEND Sources src/ValueReference.cpp)
list(APPEND Sources src/Encoder.cpp)
list(APPEND Sources src/NullEncoder.cpp)
list(APPEND Sources src/OutputChannel.cpp)
list(APPEND Sources src/NullOutput.cpp)
list(APPEND Sources src/OutputItem.cpp)
list(APPEND Sources src/Output.cpp)
list(APPEND Sources src/OutputCollection.cpp)
list(APPEND Sources src/ReportConstants.cpp)
list(APPEND Sources src/Message.cpp)
list(APPEND Sources src/Messages.cpp)
list(APPEND Sources src/ProcessMessages.cpp)
list(APPEND Sources src/MessageReporter.cpp)
list(APPEND Sources src/CommandDescription.cpp)
list(APPEND Sources src/CommandDescriptions.cpp)
list(APPEND Sources src/CommandReporter.cpp)
list(APPEND Sources src/FileDescriptorOutput.cpp)
list(APPEND Sources src/MessageHandler.cpp)
list(APPEND Sources src/StorageDataSink.cpp)
list(APPEND Sources src/InputChannel.cpp)
list(APPEND Sources src/InputScanner.cpp)
list(APPEND Sources src/InputScannerRawMessage.cpp)
list(APPEND Sources src/InputScannerDiscard.cpp)
list(APPEND Sources src/FileDescriptorInput.cpp)
list(APPEND Sources src/StringValueMapper.cpp)
list(APPEND Sources src/StringValueMapperSimple.cpp)

list(APPEND Sources src/JSONEncoder.cpp)
list(APPEND Sources src/StorageDataSinkJSON.cpp)
list(APPEND Sources src/InputScannerJSON.cpp)
list(APPEND Sources src/MessageRawJSON.cpp)

list(APPEND Sources src/Command.cpp)
list(APPEND Sources src/CommandHandler.cpp)
list(APPEND Sources src/StorageInfoCommand.cpp)
list(APPEND Sources src/GetCommand.cpp)
list(APPEND Sources src/DeleteCommand.cpp)
list(APPEND Sources src/RenameCommand.cpp)
list(APPEND Sources src/VersionCommand.cpp)
list(APPEND Sources src/NoOperationCommand.cpp)

list(APPEND Sources src/ProcessInput.cpp)
list(APPEND Sources src/Process.cpp)
list(APPEND Sources src/Processes.cpp)
list(APPEND Sources src/LocalProcess.cpp)
list(APPEND Sources src/CommandHandlerJSON.cpp)
list(APPEND Sources src/Factories.cpp)
list(APPEND Sources src/LocalProcesses.cpp)
list(APPEND Sources src/RunCommand.cpp)
list(APPEND Sources src/FeedCommand.cpp)
list(APPEND Sources src/EndFeedCommand.cpp)
list(APPEND Sources src/TerminateCommand.cpp)

list(APPEND Sources src/Options.cpp)
list(APPEND Sources src/datalackey.cpp)

add_executable( datalackey ${Sources} )
target_compile_features(datalackey PRIVATE cxx_std_14)
target_include_directories(datalackey PRIVATE src )
target_link_libraries(datalackey PRIVATE nlohmann_json::nlohmann_json )

target_compile_options(datalackey PRIVATE ${GenericOptions} )
if ("${CMAKE_BUILD_TYPE}" STREQUAL "RELEASE")
    target_compile_options(datalackey PRIVATE ${ReleaseOptions} )
endif()

if (UNIX AND NOT APPLE)
    target_link_libraries(datalackey PRIVATE Threads::Threads)
endif()

list(APPEND AlwaysSmall ${Sources})
list(REMOVE_ITEM AlwaysSmall src/RawData.cpp)
#list(REMOVE_ITEM AlwaysSmall src/FileOwner.cpp) # File I/O
list(REMOVE_ITEM AlwaysSmall src/DataGroup.cpp)
#list(REMOVE_ITEM AlwaysSmall src/Storage.cpp) # Base
#list(REMOVE_ITEM AlwaysSmall src/StorageDataSink.cpp) # Base
#list(REMOVE_ITEM AlwaysSmall src/Literal.cpp) # Simple
#list(REMOVE_ITEM AlwaysSmall src/ValueReference.cpp) # Simple
#list(REMOVE_ITEM AlwaysSmall src/Encoder.cpp) # Base
#list(REMOVE_ITEM AlwaysSmall src/NullEncoder.cpp) # Simple
#list(REMOVE_ITEM AlwaysSmall src/OutputChannel.cpp) # Base
#list(REMOVE_ITEM AlwaysSmall src/NullOutput.cpp) # Simple
#list(REMOVE_ITEM AlwaysSmall src/OutputItem.cpp) # Base
list(REMOVE_ITEM AlwaysSmall src/Output.cpp)
#list(REMOVE_ITEM AlwaysSmall src/Messages.cpp) # Simple
#list(REMOVE_ITEM AlwaysSmall src/FileDescriptorOutput.cpp) # File I/O
#list(REMOVE_ITEM AlwaysSmall src/MessageHandler.cpp) # Base
#list(REMOVE_ITEM AlwaysSmall src/StorageDataSink.cpp) # Base
#list(REMOVE_ITEM AlwaysSmall src/InputChannel.cpp) # Base
list(REMOVE_ITEM AlwaysSmall src/InputScanner.cpp)
#list(REMOVE_ITEM AlwaysSmall src/InputScannerRawMessage.cpp) # Simple
#list(REMOVE_ITEM AlwaysSmall src/InputScannerDiscard.cpp) # Simple
#list(REMOVE_ITEM AlwaysSmall src/FileDescriptorInput.cpp) # File I/O
#list(REMOVE_ITEM AlwaysSmall src/StringValueMapper.cpp) # Base
#list(REMOVE_ITEM AlwaysSmall src/StringValueMapperSimple.cpp) # Simple
list(REMOVE_ITEM AlwaysSmall src/JSONEncoder.cpp)
list(REMOVE_ITEM AlwaysSmall src/StorageDataSinkJSON.cpp)
list(REMOVE_ITEM AlwaysSmall src/InputScannerJSON.cpp)
list(REMOVE_ITEM AlwaysSmall src/MessageRawJSON.cpp)
#list(REMOVE_ITEM AlwaysSmall src/Command.cpp) # Base
#list(REMOVE_ITEM AlwaysSmall src/CommandHandler.cpp) # Base
#list(REMOVE_ITEM AlwaysSmall src/ListCommand.cpp) # Simple
#list(REMOVE_ITEM AlwaysSmall src/StorageInfoCommand.cpp) # Simple
#list(REMOVE_ITEM AlwaysSmall src/GetCommand.cpp) # Simple
#list(REMOVE_ITEM AlwaysSmall src/DeleteCommand.cpp) # Simple
#list(REMOVE_ITEM AlwaysSmall src/RenameCommand.cpp) # Simple
#list(REMOVE_ITEM AlwaysSmall src/VersionCommand.cpp) # Simple
#list(REMOVE_ITEM AlwaysSmall src/NoOperationCommand.cpp) # Simple
#list(REMOVE_ITEM AlwaysSmall src/ProcessInput.cpp) # Simple
#list(REMOVE_ITEM AlwaysSmall src/Process.cpp) # Base
#list(REMOVE_ITEM AlwaysSmall src/Processes.cpp) # Base
list(REMOVE_ITEM AlwaysSmall src/LocalProcess.cpp)
list(REMOVE_ITEM AlwaysSmall src/CommandHandlerJSON.cpp)
#list(REMOVE_ITEM AlwaysSmall src/Factories.cpp) # Simple
list(REMOVE_ITEM AlwaysSmall src/LocalProcesses.cpp)
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
    set_property(TARGET ${FULL} PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/tests/${SUBDIR}/${BASE_NAME})
endfunction()

function(new_test TEST_NAME EXE)
    add_test(NAME ${TEST_NAME} COMMAND ${EXE} $<TARGET_FILE:datalackey>)
    set_property(TEST ${TEST_NAME} PROPERTY ENVIRONMENT "PATH=${CMAKE_CURRENT_LIST_DIR}:$ENV{PATH}")
endfunction()

function(new_test_arg TEST_NAME EXE ARG)
    add_test(NAME ${TEST_NAME} COMMAND ${EXE} ${ARG} $<TARGET_FILE:datalackey>)
    set_property(TEST ${TEST_NAME} PROPERTY ENVIRONMENT "PATH=${CMAKE_CURRENT_LIST_DIR}:$ENV{PATH}")
endfunction()

function(new_test_arg3 TEST_NAME EXE ARG ARG2 ARG3)
    add_test(NAME ${TEST_NAME} COMMAND ${EXE} ${ARG} ${ARG2} ${ARG3} $<TARGET_FILE:datalackey>)
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
new_test_arg(version version.sh ${CMAKE_CURRENT_LIST_DIR}/src/Version.hpp)
add_exe(memory report.sh)
new_test_arg(messages report.sh messages)
new_test_arg(commands report.sh commands)

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
add_exe(memory null_process_id.sh)
new_test(null_id_child null_process_id.sh)

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

# Tests against found bugs.
#
add_exe(memory version_repeat.sh)
new_test(version_crash version_repeat.sh)

# Light stress tests
#
add_exe(stress overwrite.sh)
new_test_arg3(overwrite overwrite.sh 5 5 5)
add_exe(stress parallel_overwrite.sh)
new_test_arg3(parallel_overwrite parallel_overwrite.sh 10 10 10)
