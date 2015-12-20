# uGlue - Glue many languages together into a whole with ukernel-inspired RPC.
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

find_package(SDL2 REQUIRED)
find_package(Freetype REQUIRED)

add_library(renderlib STATIC widgets/Button.cpp widgets/Text_Box.cpp
                             color.cpp text.cpp)

target_link_libraries(renderlib commonlib)

target_link_libraries(renderlib ${SDL2_LIBRARIES})
target_include_directories(renderlib PUBLIC ${SDL2_INCLUDE_DIRS})

target_link_libraries(renderlib ${FREETYPE_LIBRARIES})
target_include_directories(renderlib PUBLIC ${FREETYPE_INCLUDE_DIRS})
