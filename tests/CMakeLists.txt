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

# Add the executable which runs every test.
add_executable(PpM_Tests EXCLUDE_FROM_ALL
               CollisionUtilTests.cpp LocalServerTests.cpp
               VectorTests.cpp ServerTests.cpp
               ${CMAKE_CURRENT_SOURCE_DIR}/../gtest/gtest/gtest_main.cc)

# Make sure it has the entirety of the pong support library.
# gtest headers come built-in when linking to the gtest target.
target_include_directories(PpM_Tests PUBLIC
                           ${CMAKE_CURRENT_SOURCE_DIR}/../src)
target_link_libraries(PpM_Tests ppmserv gtest)

# Now, add a check target which will automagically build the run-all testing
# executable if it has too.
add_custom_target(check)
add_custom_command(TARGET check PRE_BUILD COMMAND PpM_Tests)
