cmake_minimum_required (VERSION 3.11)
project (ring_buf)

add_library (ring_buf
    ${PROJECT_SOURCE_DIR}/src/ring_buf.c
    ${PROJECT_SOURCE_DIR}/src/ring_buf_circ.c
    ${PROJECT_SOURCE_DIR}/src/ring_buf_item.c
    ${PROJECT_SOURCE_DIR}/src/ring_buf_yield.c)
target_include_directories (ring_buf PUBLIC ${PROJECT_SOURCE_DIR}/include)

include (CTest)
enable_testing ()
file (GLOB TestsToRun "${CMAKE_SOURCE_DIR}/tests/*.c")
add_executable (RunTests Testing.c ${TestsToRun})
target_link_libraries (RunTests ring_buf)

# Register each test with CTest.
# Extract the test name from the filename.
# Tests are expected to have basenames ending with "_test".
set (TestNames)
foreach (test_to_run ${TestsToRun})
    get_filename_component (TestName ${test_to_run} NAME_WE)
    if (NOT TestName MATCHES ".+_test$")
        continue ()
    endif ()
    list (APPEND TestNames ${TestName})
    add_test (NAME ${TestName} COMMAND RunTests ${TestName})
endforeach ()
create_test_sourcelist (Tests Testing.c ${TestNames})

find_package (Doxygen)
if (DOXYGEN_FOUND)
doxygen_add_docs (doxygen ${PROJECT_SOURCE_DIR})
endif ()
