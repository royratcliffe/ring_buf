cmake_minimum_required (VERSION 3.11)
project (ring_buf)

add_library (ring_buf
    ring_buf.c
    ring_buf_item.c)
target_include_directories (ring_buf PUBLIC ${PROJECT_SOURCE_DIR})

include (CTest)
enable_testing ()
file (GLOB TestsToRun "${CMAKE_SOURCE_DIR}/tests/*.c")
add_executable (RunTests Testing.c ${TestsToRun})
target_link_libraries (RunTests ring_buf)

foreach (test_to_run ${TestsToRun})
    get_filename_component (TestName ${test_to_run} NAME_WE)
    list (APPEND TestNames ${TestName})
    add_test (NAME ${TestName} COMMAND RunTests ${TestName})
endforeach ()
create_test_sourcelist (Tests Testing.c ${TestNames})

find_package (Doxygen)
if (DOXYGEN_FOUND)
doxygen_add_docs (doxygen ${PROJECT_SOURCE_DIR})
endif ()
