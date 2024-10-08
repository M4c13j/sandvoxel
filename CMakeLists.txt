cmake_minimum_required(VERSION 3.10)
project(Sandvoxel)

#========= compiler =======
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_COMPILER /usr/bin/g++)

#set(CMAKE_BUILD_TYPE Debug)
# set(CMAKE_BUILD_TYPE Release) <- comment out such lines
# set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g") # just d
#set(CMAKE_CXX_FLAGS_DEBUG "-g -pg") # profiling
#set(CMAKE_CXX_FLAGS_DEBUG "-g -fsanitize=address -fsanitize=undefined") # memory debugging
set(CMAKE_CXX_FLAGS_RELEASE "-O3") # speeeeed

# set(USE_IMGUI false)

# Specify the paths to Dear ImGui and raylib
set(SOURCE_DIR src)
set(IMGUI_PATH /home/mac/gitrepos/imgui)
set(RAYLIB_PATH /usr/local)

# RAYLIB ===============
include_directories(
    ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/src/Simulations
        ${PROJECT_SOURCE_DIR}/src/Block
        ${PROJECT_SOURCE_DIR}/src/World
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
    ${SOURCE_DIR}/config.hpp
    ${SOURCE_DIR}/World/chunk.cpp
    ${SOURCE_DIR}/World/world.cpp
    ${SOURCE_DIR}/renderer.cpp
    ${SOURCE_DIR}/config.cpp
    src/Block/block.cpp
#        src/Block/block.hpp
        src/Block/BlockFactory.cpp
#        src/Block/BlockFactory.hpp
        src/Block/Sand.cpp
#        src/Block/Sand.hpp
        src/Block/Dirt.cpp
#        src/Block/Dirt.hpp
        src/Block/Air.cpp
        src/Block/Fluid.cpp
        src/Block/Fluid.hpp
        src/Simulations/FluidSimulation/FluidSimulation.cpp
        src/Simulations/FluidSimulation/FluidSimulation.hpp
        src/Block/Grass.cpp
        src/Block/Grass.hpp
        src/Simulations/SandSImulation/SandSimulation.cpp
        src/Simulations/SandSImulation/SandSimulation.hpp
        src/Simulations/Simulation.hpp
        #        src/Block/Air.hpp
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
