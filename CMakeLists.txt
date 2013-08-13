cmake_minimum_required(VERSION 2.8)
project(PpM)
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
  add_dependencies(doc ppm)
  set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES docs)
endif(DOXYGEN_FOUND)
