cmake_minimum_required(VERSION 3.0)
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
project(edd_USB_Barrier)

# add header path
include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/external/imgui
	${CMAKE_CURRENT_SOURCE_DIR}/external/imgui/backends
)

if (APPLE)
    include_directories(
        /usr/local/include
        /opt/local/include
        /opt/homebrew/include
        /opt/homebrew/include/SDL2
    )
elseif (WIN32)
    include_directories(
    )
elseif (UNIX AND NOT APPLE)
    include_directories(
      /usr/include/SDL2
    )
endif()

# set common source
file(GLOB SRC
    ./external/imgui/*.h
    ./external/imgui/*.cpp
)

file (GLOB PLATFORM_SRC
    ./external/imgui/backends/imgui_impl_sdl.*
    ./external/imgui/backends/imgui_impl_sdlrenderer.*
    ./src/main.cpp
)

# add link path
if (APPLE)
    link_directories(
        /usr/local/lib
        /opt/local/lib
        /opt/homebrew/lib
    )
endif()

# generate binary
add_executable(${PROJECT_NAME} ${SRC} ${PLATFORM_SRC})

# link lib, should install glfw first or prebuild lib and embed in project
if (WIN32)
    # target_link_libraries(${PROJECT_NAME}
        # d3d12.lib
        # d3dcompiler.lib
        # dxgi.lib
    # )
elseif (APPLE)
    find_library(OPENGL_LIBRARY OpenGL REQUIRED)
    find_library(SDL_LIBRARY SDL2 REQUIRED)
    find_library(COCOA_LIBRARY Cocoa REQUIRED)
    find_library(IOKIT_LIBRARY IOKit REQUIRED)
    find_library(COREVID_LIBRARY CoreVideo REQUIRED)
    message(${COCOA_LIBRARY})
    message(${IOKIT_LIBRARY})
    message(${COREVID_LIBRARY})

    target_link_libraries(${PROJECT_NAME}
        ${OPEN_GL_LIBRARY}
        ${SDL_LIBRARY}
        ${COCOA_LIBRARY}
        ${IOKIT_LIBRARY}
        ${COREVID_LIBRARY}
    )
elseif (UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME}
        GL
        SDL2
        dl
    )
endif()
