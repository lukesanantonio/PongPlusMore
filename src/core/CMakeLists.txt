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

find_package(LibUV)

add_subdirectory(common)
add_subdirectory(io)
add_subdirectory(render)
add_subdirectory(rpc)

add_executable(uglue_engine engine.cpp)
target_link_libraries(uglue_engine commonlib iolib rpclib renderlib)

install(TARGETS uglue_engine RUNTIME DESTINATION bin)

