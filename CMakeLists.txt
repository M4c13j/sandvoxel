cmake_minimum_required(VERSION 3.10)
project(RaylibImGuiExample)

set(CMAKE_CXX_STANDARD 17)

# set(USE_IMGUI false)

set(SOURCE_DIR src)
# Specify the paths to Dear ImGui and raylib
set(IMGUI_PATH /home/mac/gitrepos/imgui)
set(RAYLIB_PATH /usr/local)

# RAYLIB ===============
include_directories(
    ${PROJECT_SOURCE_DIR}/include
)
# Bez add_library, bo jest już zbudowany, więc target_link sam znajdzie


# IMGUI ===================
# Include directories
if( DEFINED USE_IMGUI )
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

    target_link_libraries($(PROJECT_NAME) imgui)
endif( DEFINED USE_IMGUI )


# Add the executable, tutaj są pliki z projektu
add_executable(${PROJECT_NAME}
    ${SOURCE_DIR}/main.cpp
    ${SOURCE_DIR}/player.cpp
    ${SOURCE_DIR}/block.cpp
    ${SOURCE_DIR}/chunk.cpp
    ${SOURCE_DIR}/config.cpp
    ${SOURCE_DIR}/map.cpp
    ${SOURCE_DIR}/renderer.cpp
    # ${SOURCE_DIR}/.cpp
    # ${SOURCE_DIR}/block.cpp
)

# Link libraries, lista bibliotek linkowanych, zdefiniowanych tu lub globalnie.
target_link_libraries(${PROJECT_NAME}
    raylib
)

# APPLE related ============
if (APPLE)
    set(CMAKE_CXX_COMPILER "/opt/homebrew/bin/g++-14") #arm Mac stuff
    list(APPEND CMAKE_PREFIX_PATH "/usr/local" "/opt/homebrew")
    find_package(raylib REQUIRED)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()


# NO WAY. thanks chat GPT
