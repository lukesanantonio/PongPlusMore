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

find_package(SDL2 REQUIRED)
find_package(Freetype REQUIRED)

add_library(ppmcore STATIC common/volume.cpp
                           io/external_io.cpp
                           io/ipc.cpp
                           io/Logger.cpp
                           io/net.cpp
                           req/req.cpp
                           widgets/Button.cpp
                           widgets/Text_Box.cpp
                           text.cpp
                           utility.cpp)

target_link_libraries(ppmcore ${SDL2_LIBRARIES} ${FREETYPE_LIBRARIES})
target_link_libraries(ppmcore jsoncpp)
target_include_directories(ppmcore PUBLIC ${SDL2_INCLUDE_DIRS}
                                          ${FREETYPE_INCLUDE_DIRS})

add_executable(pong_engine engine.cpp)
target_link_libraries(pong_engine ppmcore)

install(TARGETS pong_engine RUNTIME DESTINATION bin)

