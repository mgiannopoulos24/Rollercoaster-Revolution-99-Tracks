# iOS specific settings
set(CMAKE_SYSTEM_NAME iOS)
set(CMAKE_OSX_ARCHITECTURES "arm64")
set(CMAKE_OSX_DEPLOYMENT_TARGET "13.0")

# iOS specific flags for SDL2
add_compile_options("-fobjc-arc")
