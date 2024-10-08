####
# F prime CMakeLists.txt:
#
# SOURCE_FILES: combined list of source and autocoding files
# MOD_DEPS: (optional) module dependencies
#
####
add_custom_target("${FPRIME_CURRENT_MODULE}")

######## File Stub ########
set(SOURCE_FILES
    "${CMAKE_CURRENT_LIST_DIR}/File.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/DefaultFile.cpp"
)
register_fprime_module(Os_File_Stub)
register_fprime_implementation(Os/File Os_File_Stub)

######## Mutex Stub ########
set(SOURCE_FILES
    "${CMAKE_CURRENT_LIST_DIR}/Mutex.cpp"
)
register_fprime_module(Os_Mutex_Stub)
register_fprime_implementation(Os/Mutex Os_Mutex_Stub "${CMAKE_CURRENT_LIST_DIR}/DefaultMutex.cpp")


# Remainder of file is specific to UTs
if (NOT BUILD_TESTING)
    return()
endif ()

#### File Stub Testing ####
set(SOURCE_FILES
    "${CMAKE_CURRENT_LIST_DIR}/test/File.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test/DefaultFile.cpp"
)
register_fprime_module(Os_File_Test_Stub)
register_fprime_implementation(Os/File Os_File_Test_Stub)

set(UT_SOURCE_FILES
    "${CMAKE_CURRENT_LIST_DIR}/test/ut/StubFileTests.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../test/ut/file/CommonTests.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../test/ut/file/FileRules.cpp"
)
set(UT_MOD_DEPS
    Os
    Os_Models
    Os_Test_File_SyntheticFileSystem
    STest
)
choose_fprime_implementation(Os/File Os_File_Test_Stub)
register_fprime_ut(StubFileTest)

#### Console Stub Testing ####
set(SOURCE_FILES
        "${CMAKE_CURRENT_LIST_DIR}/Console.cpp"
)
set(HEADER_FILES
        "${CMAKE_CURRENT_LIST_DIR}/Console.hpp"
)
set(MOD_DEPS)
register_fprime_module(Os_Console_Stub)
register_fprime_implementation(Os/Console Os_Console_Stub "${CMAKE_CURRENT_LIST_DIR}/DefaultConsole.cpp")

set(SOURCE_FILES
        "${CMAKE_CURRENT_LIST_DIR}/test/Console.cpp"
)
set(MOD_DEPS)
register_fprime_module(Os_Console_Test_Stub)
register_fprime_implementation(Os/Console Os_Console_Test_Stub "${CMAKE_CURRENT_LIST_DIR}/test/DefaultConsole.cpp")

set(UT_SOURCE_FILES
        "${CMAKE_CURRENT_LIST_DIR}/test/ut/StubConsoleTests.cpp"
)
set(UT_MOD_DEPS)
choose_fprime_implementation(Os/Console Os_Console_Test_Stub)
register_fprime_ut(StubConsoleTest)


#### Task Stub Testing ####
set(SOURCE_FILES
        "${CMAKE_CURRENT_LIST_DIR}/test/Task.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/test/DefaultTask.cpp"
)
register_fprime_module(Os_Task_Test_Stub)
register_fprime_implementation(Os/Task Os_Task_Test_Stub)

set(UT_SOURCE_FILES
        "${CMAKE_CURRENT_LIST_DIR}/test/ut/StubTaskTests.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/../test/ut/task/CommonTests.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/../test/ut/task/TaskRules.cpp"
)
set(UT_MOD_DEPS
        Fw/Time
        Os
        Os_Models
        STest
)
choose_fprime_implementation(Os/Task Os_Task_Test_Stub)
register_fprime_ut(StubTaskTest)

#### Mutex Stub Testing ####
set(SOURCE_FILES
        "${CMAKE_CURRENT_LIST_DIR}/test/Mutex.cpp"
)
register_fprime_module(Os_Mutex_Test_Stub)
register_fprime_implementation(Os/Mutex Os_Mutex_Test_Stub "${CMAKE_CURRENT_LIST_DIR}/test/DefaultMutex.cpp")

set(UT_SOURCE_FILES
        "${CMAKE_CURRENT_LIST_DIR}/test/ut/StubMutexTests.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/../test/ut/mutex/CommonTests.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/../test/ut/mutex/MutexRules.cpp"
)
set(UT_MOD_DEPS
        Os
        Os_Models
        STest
)
choose_fprime_implementation(Os/Mutex Os_Mutex_Test_Stub)
register_fprime_ut(StubMutexTest)