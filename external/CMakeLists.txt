# GoogleTest on UNIX requires pthread
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -pthread)
set(BUILD_GMOCK OFF)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory(googletest)

add_subdirectory(glad)
add_subdirectory(glfw)
add_subdirectory(glm)

# We merge imgui and implot into one library to make including/linking easier
include_directories(imgui)
include_directories(implot)
add_library(imgui
        imgui/backends/imgui_impl_glfw.cpp
        imgui/backends/imgui_impl_opengl3.cpp
        imgui/imgui_demo.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_tables.cpp
        imgui/imgui_widgets.cpp
        imgui/imgui.cpp
        implot/implot.cpp
        implot/implot_demo.cpp
        implot/implot_items.cpp)
target_link_libraries(imgui glfw glad)
target_include_directories(imgui PUBLIC imgui implot)
