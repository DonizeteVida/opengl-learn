cmake_minimum_required(VERSION 3.5.0)
project(opengl-learn VERSION 0.1.0 LANGUAGES C CXX)

set(GLFW_BUILD_WAYLAND OFF)
# libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev libgl-dev
set(GLFW_BUILD_X11 ON)

add_subdirectory(subprojects)

add_executable(main main.cpp)

target_link_libraries(main glad)
target_link_libraries(main glfw)