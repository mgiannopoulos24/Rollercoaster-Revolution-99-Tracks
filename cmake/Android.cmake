# Android specific settings
set(CMAKE_SYSTEM_NAME Android)
set(CMAKE_ANDROID_ARCHIVE_LIBRARY TRUE)
set(CMAKE_ANDROID_STL_TYPE c++_shared)

# ABI filters
set(CMAKE_ANDROID_ABI "arm64-v8a;armeabi-v7a;x86_64")

# Min SDK version
set(CMAKE_ANDROID_API_LEVEL 21)

# Add Android-specific source files if any
# (e.g., JNI wrapper)
