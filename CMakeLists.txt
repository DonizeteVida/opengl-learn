cmake_minimum_required(VERSION 3.5.0...3.31.5)
project(opengl-learn VERSION 0.1.0 LANGUAGES C CXX)

if (APPLE)
    set(CMAKE_CXX_STANDARD 17)

elseif (UNIX)
    set(GLFW_BUILD_WAYLAND OFF)
    # libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev libgl-dev
    set(GLFW_BUILD_X11 ON)
endif()

add_subdirectory(subprojects)

link_libraries(glad glfw shaders)

add_subdirectory(src)

add_executable(main main.cpp)