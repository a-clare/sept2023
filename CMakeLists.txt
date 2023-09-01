cmake_minimum_required(VERSION 3.19)
project(sept2023)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(external)

add_executable(sept2023 main.cpp)
target_link_libraries(sept2023 glm glfw imgui glad)