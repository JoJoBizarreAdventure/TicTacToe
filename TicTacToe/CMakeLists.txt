file(COPY font/menuFont.ttf
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/font)

add_executable(${PROJECT_NAME} app.cpp window.cpp)

# ui part
target_include_directories(${PROJECT_NAME} PRIVATE ui/include)
target_sources(${PROJECT_NAME} PRIVATE ui/uiBase.cpp ui/ui.cpp ui/chessboard.cpp ui/grid.cpp ui/sidebar.cpp ui/anim.cpp)

# player part
target_include_directories(${PROJECT_NAME} PRIVATE player/include)
target_sources(${PROJECT_NAME} PRIVATE
        player/humanPlayer.cpp player/aiPlayer.cpp player/simpleAIPlayer.cpp player/minMaxAIPlayer.cpp)

# task part
target_include_directories(${PROJECT_NAME} PRIVATE task/include)
target_sources(${PROJECT_NAME} PRIVATE task/task.cpp)

link_blend2d(${PROJECT_NAME})
link_lua(${PROJECT_NAME})
link_glad(${PROJECT_NAME})
link_glfw(${PROJECT_NAME})
link_glm(${PROJECT_NAME})
link_fmt(${PROJECT_NAME})

file(COPY lua DESTINATION ${CMAKE_CURRENT_BINARY_DIR})