# PpM - Pong Plus More - A pong clone full of surprises written with C++11.
# Copyright (C) 2013  Luke San Antonio
#
# You can contact me (Luke San Antonio) at lukesanantonio@gmail.com!
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

# Init CMake stuff.
cmake_minimum_required(VERSION 2.8)
project(PpM)

# Make sure to show our FindSDL2.cmake some love.
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)

# Find boost
find_package(Boost 1.53.0 REQUIRED)

macro(add_data_target target)
  add_custom_target(${target} ${ARGV1})
endmacro()
macro(add_target_data target filename)
  add_custom_command(TARGET ${target} COMMAND ${CMAKE_COMMAND}
                     ARGS -E copy_if_different
                     ${CMAKE_CURRENT_SOURCE_DIR}/${filename}
                     ${CMAKE_CURRENT_BINARY_DIR}/${filename})
  install(FILES ${filename} DESTINATION share/PpM)
endmacro()
macro(add_target_directory target dirname)
  add_custom_command(TARGET ${target} COMMAND ${CMAKE_COMMAND}
                     ARGS -E copy_directory
                     ${CMAKE_CURRENT_SOURCE_DIR}/${dirname}
                     ${CMAKE_CURRENT_BINARY_DIR}/${dirname})
  install(DIRECTORY ${dirname} DESTINATION share/PpM)
endmacro()

# Actually build our project, or at least declare it.
add_subdirectory(gtest)
add_subdirectory(jsoncpp)
add_subdirectory(plugins)
add_subdirectory(resources)
add_subdirectory(src)
add_subdirectory(tests)

# Set up a custom 'doc' target.
find_package(Doxygen)
if(DOXYGEN_FOUND)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
                 ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
  add_custom_target(doc
                    ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
                    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                    COMMENT "Generating documentation with doxygen"
                    VERBATIM)
  set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES docs)
endif(DOXYGEN_FOUND)
