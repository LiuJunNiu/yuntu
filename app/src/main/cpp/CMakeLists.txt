# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.10.2)

# Declares and names the project.

project("yuntunative")

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

ADD_DEFINITIONS(-DOS_ANDROID)
file(GLOB src_android_wrapper ${CMAKE_CURRENT_SOURCE_DIR}/android_wrapper/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/android_wrapper/*.c)

include_directories(${PROJECT_SOURCE_DIR}/qcloud_iot_c_sdk/include
        ${PROJECT_SOURCE_DIR}/qcloud_iot_c_sdk/include/exports
        ${PROJECT_SOURCE_DIR}/qcloud_iot_c_sdk/sdk_src/internal_inc
        ${PROJECT_SOURCE_DIR}/android_wrapper
        )

add_library(iotlink
        SHARED
        IMPORTED
        )

set_target_properties(iotlink
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/../libs/${ANDROID_ABI}/libiotlink.so)

add_library(android_wrapper
        SHARED
        ${src_android_wrapper})

add_library( # Sets the name of the library.
        native-lib

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        native-lib.cpp)

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.
find_library( # Sets the name of the path variable.
        android_wrapper

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log )

find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
        android_wrapper

        # Links the target library to the log library
        # included in the NDK.
        ${log-lib} iotlink)

target_link_libraries( # Specifies the target library.
        native-lib

        # Links the target library to the log library
        # included in the NDK.
        ${log-lib} iotlink android_wrapper)