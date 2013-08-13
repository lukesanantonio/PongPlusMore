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
