cmake_minimum_required(VERSION 2.6)
project(ultimate_pong)
add_subdirectory(src)
find_package(Doxygen)
if(DOXYGEN_FOUND)
  configure_file(${CMAKE_SOURCE_DIR}/Doxyfile.in
                 ${CMAKE_BINARY_DIR}/Doxyfile)
  add_custom_target(doc
                    ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/Doxyfile
                    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                    COMMENT "Generating documentation with doxygen"
                    VERBATIM)
endif(DOXYGEN_FOUND)
