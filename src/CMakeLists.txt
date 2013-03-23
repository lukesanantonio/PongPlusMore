find_package(SDL)
add_executable(ultimate_pong Game.cpp)
set_target_properties(ultimate_pong PROPERTIES COMPILE_FLAGS "-std=c++11")
target_link_libraries(ultimate_pong ${SDL_LIBRARY})
