find_package(SDL)
add_executable(ultimate_pong Game.cpp)
target_link_libraries(ultimate_pong ${SDL_LIBRARY})
install(TARGETS ultimate_pong
        RUNTIME DESTINATION bin)
