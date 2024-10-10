cmake_minimum_required (VERSION 3.28)
project (ring_buf)

add_library (ring_buf
    ring_buf.c
    ring_buf_item.c)

include (CTest)
enable_testing ()
set (TestsToRun
    ring_buf_test.c
    ring_buf_item_test.c)
create_test_sourcelist (Tests Testing.c ${TestsToRun})

add_executable (RunTests ${Tests})
target_link_libraries (RunTests ring_buf)

foreach (test_to_run ${TestsToRun})
    get_filename_component (TestName ${test_to_run} NAME_WE)
    add_test (NAME ${TestName} COMMAND RunTests ${TestName})
endforeach ()
