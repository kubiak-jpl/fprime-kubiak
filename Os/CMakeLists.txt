####
# F prime CMakeLists.txt:
#
# SOURCE_FILES: combined list of source and autocoding files
# MOD_DEPS: (optional) module dependencies
#
####
add_fprime_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Models")
add_fprime_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Stub")
add_fprime_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Posix")

set(SOURCE_FILES
    "${CMAKE_CURRENT_LIST_DIR}/test/ut/file/SyntheticFileSystem.cpp"
)
register_fprime_module(Os_Test_File_SyntheticFileSystem)


# Basic module dependencies
set(MOD_DEPS
  ${CMAKE_THREAD_LIBS_INIT}
  Fw/Cfg
  Fw/Types
  Fw/Logger
  Utils/Hash
)
# Basic source files used in every OSAL layer.  Contains common code and helpers.
set(SOURCE_FILES
    "${CMAKE_CURRENT_LIST_DIR}/IntervalTimerCommon.cpp"
#    "${CMAKE_CURRENT_LIST_DIR}/TaskCommon.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/QueueCommon.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/IPCQueueCommon.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/SimpleQueueRegistry.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/MemCommon.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/ValidateFileCommon.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/ValidatedFile.cpp"

    # Refactored common files
    "${CMAKE_CURRENT_LIST_DIR}/File.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/Task.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/Mutex.cpp"
)
# Check for default logger
if (NOT FPRIME_DISABLE_DEFAULT_LOGGER)
    list(APPEND SOURCE_FILES
        "${CMAKE_CURRENT_LIST_DIR}/LogDefault.cpp"
    )
endif()
# Posix systems typically share these
if (FPRIME_USE_POSIX)
    list(APPEND SOURCE_FILES
        "${CMAKE_CURRENT_LIST_DIR}/Console.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/Pthreads/Queue.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/Pthreads/BufferQueueCommon.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/Pthreads/PriorityBufferQueue.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/Pthreads/MaxHeap/MaxHeap.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/Linux/InterruptLock.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/Linux/WatchdogTimer.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/Posix/IntervalTimer.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/Linux/FileSystem.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/Linux/Directory.cpp"
    )
endif()
# Darwin IPC queue implementation
if(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
  list(APPEND SOURCE_FILES
    "${CMAKE_CURRENT_LIST_DIR}/MacOs/IPCQueueStub.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/MacOs/SystemResources.cpp"
  )
# Linux IPC queues implementation
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  list(APPEND SOURCE_FILES
    "${CMAKE_CURRENT_LIST_DIR}/Posix/IPCQueue.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/Posix/LocklessQueue.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Linux/SystemResources.cpp"
  )
  # Shared libraries need an -rt dependency for mq libs
  if (BUILD_SHARED_LIBS)
      list(APPEND MOD_DEPS "-lrt")
  endif()
endif()


# If baremetal scheduler is set, remove the previous task files and add in the Baremetal variant
if (FPRIME_USE_BAREMETAL_SCHEDULER)
    add_fprime_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Baremetal/TaskRunner/")
    foreach (ITER_ITEM IN LISTS SOURCE_FILES)
        if (ITER_ITEM MATCHES "Task\\.cpp$")
            list(REMOVE_ITEM SOURCE_FILES "${ITER_ITEM}")
        endif()
    endforeach()
    list(APPEND SOURCE_FILES "${CMAKE_CURRENT_LIST_DIR}/Baremetal/FileSystem.cpp")
    list(APPEND SOURCE_FILES "${CMAKE_CURRENT_LIST_DIR}/Baremetal/IntervalTimer.cpp")
    list(APPEND SOURCE_FILES "${CMAKE_CURRENT_LIST_DIR}/Baremetal/Mutex.cpp")
    list(APPEND SOURCE_FILES "${CMAKE_CURRENT_LIST_DIR}/Baremetal/Queue.cpp")
    list(APPEND SOURCE_FILES "${CMAKE_CURRENT_LIST_DIR}/Baremetal/SystemResources.cpp")
endif()
register_fprime_module()

require_fprime_implementation(Os/File)
# require_fprime_implementation(Os/Task) # should be added in
require_fprime_implementation(Os/Mutex) 


### UTS ### Note: 3 separate UTs registered here.
set(UT_SOURCE_FILES
  "${CMAKE_CURRENT_LIST_DIR}/test/ut/OsQueueTest.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/test/ut/OsTestMain.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/test/ut/IntervalTimerTest.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/test/ut/OsFileSystemTest.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/test/ut/OsValidateFileTest.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/test/ut/OsSystemResourcesTest.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/test/ut/OsMutexBasicLockableTest.cpp"
)
register_fprime_ut()
if (BUILD_TESTING)
    foreach (TEST IN ITEMS StubFileTest PosixFileTest StubMutexTest PosixMutexTest) # add? : StubTaskTest PosixTaskTest
       if (TARGET "${TEST}")
           add_dependencies("Os_ut_exe" "${TEST}")
       endif()
    endforeach()
endif()
# Second UT Pthreads
set(UT_SOURCE_FILES
  "${CMAKE_CURRENT_LIST_DIR}/Pthreads/test/ut/BufferQueueTest.cpp"
)
register_fprime_ut("Os_pthreads")

# Third  UT Pthreads MAX Heap
set(UT_SOURCE_FILES
  "${CMAKE_CURRENT_LIST_DIR}/Pthreads/MaxHeap/test/ut/MaxHeapTest.cpp"
)
register_fprime_ut("Os_pthreads_max_heap")
