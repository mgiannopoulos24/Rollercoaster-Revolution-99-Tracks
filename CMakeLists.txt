cmake_minimum_required(VERSION 3.20)
project(RollercoasterRevolution99 VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include compiler flags
include(cmake/CompilerFlags.cmake)

# Find dependencies
include(cmake/FindSDL2.cmake)

# Platform specific configurations
if(EMSCRIPTEN)
    include(cmake/Emscripten.cmake)
elseif(ANDROID)
    include(cmake/Android.cmake)
elseif(APPLE AND iOS)
    include(cmake/iOS.cmake)
endif()

# Generate version header from template
set(RR99_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(RR99_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(RR99_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(RR99_VERSION_TWEAK 0)

# Get git commit hash if available
execute_process(
    COMMAND git log -1 --format=%h
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE RR99_GIT_COMMIT
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET
)
if(NOT RR99_GIT_COMMIT)
    set(RR99_GIT_COMMIT "unknown")
endif()

set(RR99_VERSION_STRING "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")

configure_file(
    resources/version.h.in
    ${CMAKE_BINARY_DIR}/generated/version.h
    @ONLY
)

# Collect source files
file(GLOB_RECURSE SRC_FILES
    "src/*.cpp"
)

# For now, just a simple executable
add_executable(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE
    src
    src/core
    src/db
    src/physics
    src/track
    src/game
    src/input
    src/audio
    src/render
    src/render/ui
    src/platform
    src/tools
    ${CMAKE_BINARY_DIR}/generated
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    SDL2::SDL2
    SDL2::SDL2_image
    SDL2::SDL2_mixer
    SDL2::SDL2_ttf
    SQLite3::SQLite3
)
