find_package(SDL REQUIRED)
add_executable(ultimate_pong Game.cpp MenuGameState.cpp Button.cpp)
target_link_libraries(ultimate_pong ${SDL_LIBRARY})
include_directories(${SDL_INCLUDE_DIR})
install(TARGETS ultimate_pong
        RUNTIME DESTINATION bin)
