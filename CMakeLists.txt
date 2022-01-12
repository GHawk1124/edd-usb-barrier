cmake_minimum_required(VERSION 3.0)
# set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
add_executable(btc.bin external/imgui/misc/fonts/binary_to_compressed_c.cpp)
execute_process(
  COMMAND mkdir -p ${CMAKE_CURRENT_SOURCE_DIR}/assets/bin_fonts
)
execute_process(
  COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/build/btc.bin ${CMAKE_CURRENT_SOURCE_DIR}/assets/SourceCodePro/SourceCodePro-Regular.ttf SourceCodePro_Regular
  OUTPUT_FILE ${CMAKE_CURRENT_SOURCE_DIR}/assets/bin_fonts/SourceCodePro-Regular.cpp
)
project(edd_USB_Barrier)
# SET (CMAKE_C_COMPILER_WORKS 1)
# SET (CMAKE_CXX_COMPILER_WORKS 1)

# add header path
include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/external/imgui
	${CMAKE_CURRENT_SOURCE_DIR}/external/imgui/backends
	${CMAKE_CURRENT_SOURCE_DIR}/external/imgui/misc/freetype
	${CMAKE_CURRENT_SOURCE_DIR}/assets/bin_fonts
)

if (APPLE)
    include_directories(
        /usr/local/include
        /opt/local/include
        /opt/homebrew/include
        /opt/homebrew/include/GL
        /opt/homebrew/include/GLFW
        /opt/homebrew/include/freetype2
    )
elseif (WIN32)
    include_directories(
    )
elseif (UNIX AND NOT APPLE)
    include_directories(
      /usr/local/include/SDL2
      /usr/local/include/freetype2
    )
endif()

# set common source
file(GLOB SRC
    ./external/imgui/*.h
    ./external/imgui/*.cpp
    ./external/imgui/misc/freetype/*.h
    ./external/imgui/misc/freetype/*.cpp
)

file (GLOB PLATFORM_SRC
    ./external/imgui/backends/imgui_impl_glfw.*
    ./external/imgui/backends/imgui_impl_opengl3.*
    ./src/*.cpp
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
    find_library(GLES_LIBRARY GLESv2 REQUIRED)
    find_library(GLFW_LIBRARY GLFW REQUIRED)
    find_library(COCOA_LIBRARY Cocoa REQUIRED)
    find_library(IOKIT_LIBRARY IOKit REQUIRED)
    find_library(COREVID_LIBRARY CoreVideo REQUIRED)
    find_library(FREETYPE_LIBRARY freetype REQUIRED)
    message(${COCOA_LIBRARY})
    message(${IOKIT_LIBRARY})
    message(${COREVID_LIBRARY})

    target_link_libraries(${PROJECT_NAME}
        ${OPEN_GL_LIBRARY}
        ${GLES_LIBRARY}
        ${GLFW_LIBRARY}
        ${COCOA_LIBRARY}
        ${IOKIT_LIBRARY}
        ${COREVID_LIBRARY}
        ${FREETYPE_LIBRARY}
    )
elseif (UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME}
        GL
        glfw
        dl
        freetype
    )
endif()
