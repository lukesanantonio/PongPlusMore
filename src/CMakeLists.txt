find_package(SDL REQUIRED)
find_package(Freetype REQUIRED)
add_executable(ppm Game.cpp GameState.cpp common.cpp render_text.cpp
Label.cpp Button.cpp CachedSurface.cpp GameStates/MenuGameState.cpp Paddle.cpp
GameSettings.cpp Ball.cpp jsoncpp/jsoncpp.cpp)
target_link_libraries(ppm ${SDL_LIBRARY} ${FREETYPE_LIBRARIES})

include_directories(${SDL_INCLUDE_DIR} ${FREETYPE_INCLUDE_DIRS}
${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/jsoncpp)

install(TARGETS ppm
        RUNTIME DESTINATION bin)
