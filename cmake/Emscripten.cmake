# Emscripten specific settings
set(CMAKE_EXECUTABLE_SUFFIX ".html")

# Emscripten settings for SDL2 and IDBFS
add_link_options("-s" "USE_SDL=2")
add_link_options("-s" "USE_SDL_image=2")
add_link_options("-s" "USE_SDL_mixer=2")
add_link_options("-s" "USE_SDL_ttf=2")
add_link_options("-s" "IDBFS=1")
add_link_options("-s" "ALLOW_MEMORY_GROWTH=1")
add_link_options("-s" "ASYNCIFY=1")
