find_package(PkgConfig QUIET)

# SDL2
if(PkgConfig_FOUND)
    pkg_check_modules(SDL2_PKG sdl2 IMPORTED_TARGET)
endif()

if(SDL2_PKG_FOUND)
    add_library(SDL2::SDL2 ALIAS PkgConfig::SDL2_PKG)
else()
    find_path(SDL2_INCLUDE_DIR NAMES SDL.h PATH_SUFFIXES SDL2)
    find_library(SDL2_LIBRARY NAMES SDL2)
    include(FindPackageHandleStandardArgs)
    find_package_handle_standard_args(SDL2 REQUIRED_VARS SDL2_LIBRARY SDL2_INCLUDE_DIR)
    if(SDL2_FOUND AND NOT TARGET SDL2::SDL2)
        add_library(SDL2::SDL2 UNKNOWN IMPORTED)
        set_target_properties(SDL2::SDL2 PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${SDL2_INCLUDE_DIR}"
            IMPORTED_LOCATION "${SDL2_LIBRARY}"
        )
    endif()
endif()

# SDL2_image
if(PkgConfig_FOUND)
    pkg_check_modules(SDL2_IMAGE_PKG SDL2_image IMPORTED_TARGET)
endif()

if(SDL2_IMAGE_PKG_FOUND)
    add_library(SDL2::SDL2_image ALIAS PkgConfig::SDL2_IMAGE_PKG)
else()
    find_path(SDL2_IMAGE_INCLUDE_DIR NAMES SDL_image.h PATH_SUFFIXES SDL2)
    find_library(SDL2_IMAGE_LIBRARY NAMES SDL2_image)
    include(FindPackageHandleStandardArgs)
    find_package_handle_standard_args(SDL2_image REQUIRED_VARS SDL2_IMAGE_LIBRARY SDL2_IMAGE_INCLUDE_DIR)
    if(SDL2_IMAGE_FOUND AND NOT TARGET SDL2::SDL2_image)
        add_library(SDL2::SDL2_image UNKNOWN IMPORTED)
        set_target_properties(SDL2::SDL2_image PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${SDL2_IMAGE_INCLUDE_DIR}"
            IMPORTED_LOCATION "${SDL2_IMAGE_LIBRARY}"
        )
    endif()
endif()

# SDL2_mixer
if(PkgConfig_FOUND)
    pkg_check_modules(SDL2_MIXER_PKG SDL2_mixer IMPORTED_TARGET)
endif()

if(SDL2_MIXER_PKG_FOUND)
    add_library(SDL2::SDL2_mixer ALIAS PkgConfig::SDL2_MIXER_PKG)
else()
    find_path(SDL2_MIXER_INCLUDE_DIR NAMES SDL_mixer.h PATH_SUFFIXES SDL2)
    find_library(SDL2_MIXER_LIBRARY NAMES SDL2_mixer)
    include(FindPackageHandleStandardArgs)
    find_package_handle_standard_args(SDL2_mixer REQUIRED_VARS SDL2_MIXER_LIBRARY SDL2_MIXER_INCLUDE_DIR)
    if(SDL2_MIXER_FOUND AND NOT TARGET SDL2::SDL2_mixer)
        add_library(SDL2::SDL2_mixer UNKNOWN IMPORTED)
        set_target_properties(SDL2::SDL2_mixer PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${SDL2_MIXER_INCLUDE_DIR}"
            IMPORTED_LOCATION "${SDL2_MIXER_LIBRARY}"
        )
    endif()
endif()

# SDL2_ttf
if(PkgConfig_FOUND)
    pkg_check_modules(SDL2_TTF_PKG SDL2_ttf IMPORTED_TARGET)
endif()

if(SDL2_TTF_PKG_FOUND)
    add_library(SDL2::SDL2_ttf ALIAS PkgConfig::SDL2_TTF_PKG)
else()
    find_path(SDL2_TTF_INCLUDE_DIR NAMES SDL_ttf.h PATH_SUFFIXES SDL2)
    find_library(SDL2_TTF_LIBRARY NAMES SDL2_ttf)
    include(FindPackageHandleStandardArgs)
    find_package_handle_standard_args(SDL2_ttf REQUIRED_VARS SDL2_TTF_LIBRARY SDL2_TTF_INCLUDE_DIR)
    if(SDL2_TTF_FOUND AND NOT TARGET SDL2::SDL2_ttf)
        add_library(SDL2::SDL2_ttf UNKNOWN IMPORTED)
        set_target_properties(SDL2::SDL2_ttf PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${SDL2_TTF_INCLUDE_DIR}"
            IMPORTED_LOCATION "${SDL2_TTF_LIBRARY}"
        )
    endif()
endif()

# SQLite3
find_package(SQLite3 QUIET)
if(SQLite3_FOUND AND NOT TARGET SQLite3::SQLite3)
    add_library(SQLite3::SQLite3 UNKNOWN IMPORTED)
    set_target_properties(SQLite3::SQLite3 PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${SQLite3_INCLUDE_DIRS}"
        IMPORTED_LOCATION "${SQLite3_LIBRARIES}"
    )
endif()
