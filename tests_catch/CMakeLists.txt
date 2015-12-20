
# Copyright (C) 2015 Luke San Antonio
# All rights reserved.

macro(add_tests subdirectory)
  foreach(filename ${ARGN})
    set(UGLUE_TEST_FILES ${UGLUE_TEST_FILES} "${subdirectory}/${filename}")
  endforeach()
endmacro()

add_tests(common cache.cpp idgen.cpp utility.cpp vector.cpp volume.cpp)
add_tests(plugin req_dispatcher.cpp)
add_executable(run_all_tests main.cpp ${UGLUE_TEST_FILES})

target_include_directories(run_all_tests PUBLIC ${CMAKE_SOURCE_DIR}
                           ${CMAKE_CURRENT_SOURCE_DIR}/../src/core)
target_link_libraries(run_all_tests PUBLIC commonlib rpclib)
