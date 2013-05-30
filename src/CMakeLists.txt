find_package(SDL REQUIRED)
find_package(Freetype REQUIRED)
add_executable(ultimate_pong Game.cpp GameStates/MenuGameState.cpp GameStates/SinglePlayerGameState.cpp Button.cpp Label.cpp render_text.cpp EventSignals.cpp Paddle.cpp)
target_link_libraries(ultimate_pong ${SDL_LIBRARY} ${FREETYPE_LIBRARIES})
include_directories(${SDL_INCLUDE_DIR} ${FREETYPE_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})
install(TARGETS ultimate_pong
        RUNTIME DESTINATION bin)
