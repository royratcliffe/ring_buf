cmake_minimum_required (VERSION 3.9)
project (ring_buf)

add_library (ring_buf
    ring_buf.c
    ring_buf_item.c)

include (CTest)
enable_testing ()
set (TestsToRun
    ring_buf_test.c
    ring_buf_size_max_test.c
    ring_buf_float_4_test.c
    ring_buf_circular_float_test.c
    ring_buf_item_test.c)
create_test_sourcelist (Tests Testing.c ${TestsToRun})

add_executable (RunTests ${Tests})
target_link_libraries (RunTests ring_buf)

foreach (test_to_run ${TestsToRun})
    get_filename_component (TestName ${test_to_run} NAME_WE)
    add_test (NAME ${TestName} COMMAND RunTests ${TestName})
endforeach ()

find_package(Doxygen)
if (DOXYGEN_FOUND)
doxygen_add_docs(doxygen ${PROJECT_SOURCE_DIR})
endif ()
