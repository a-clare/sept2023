cmake_minimum_required(VERSION 3.19)
project(sept2023)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(external)
# We need to include eigen here (at the highest level cmakelists) for each target
# to have access to eigen headers
include_directories(external/eigen-3.4.0)

add_executable(sept2023
        main.cpp
        camera.cpp
        shader.cpp
        robot.cpp)
target_link_libraries(sept2023 glm glfw imgui glad)