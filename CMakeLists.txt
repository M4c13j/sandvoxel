cmake_minimum_required(VERSION 3.10)
project(RaylibImGuiExample)

set(CMAKE_CXX_STANDARD 17)

# Specify the paths to Dear ImGui and raylib
set(IMGUI_PATH /home/mac/gitrepos/imgui)
set(RAYLIB_PATH /usr/local)

# RAYLIB ===============
include_directories(
    ${RAYLIB_PATH}/include
)
# Bez add_library, bo jest już zbudowany, więc target_link sam znajdzie

# IMGUI =============
# Include directories
include_directories(
    ${IMGUI_PATH}
    ${IMGUI_PATH}/backends
)

# Add Dear ImGui source files
add_library(imgui STATIC
    ${IMGUI_PATH}/imgui.cpp
    ${IMGUI_PATH}/imgui_draw.cpp
    ${IMGUI_PATH}/imgui_demo.cpp
    ${IMGUI_PATH}/imgui_widgets.cpp
    ${IMGUI_PATH}/imgui_tables.cpp # Added this line for Tables support
    ${IMGUI_PATH}/backends/imgui_impl_glfw.cpp
    ${IMGUI_PATH}/backends/imgui_impl_opengl3.cpp
)


# Add the executable, tutaj są pliki z projektu
add_executable(${PROJECT_NAME}
    main.cpp
)


# APPLE related ============
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()

# Link libraries, lista bibliotek linkowanych, zdefiniowanych tu lub globalnie.
target_link_libraries(${PROJECT_NAME}
    imgui
    raylib
    GL
    glfw
)

# NO WAY. thanks chat GPT
