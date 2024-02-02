# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html.
# For more examples on how to use CMake, see https://github.com/android/ndk-samples.

# Sets the minimum CMake version required for this project.
cmake_minimum_required(VERSION 3.22.1)

# Declares the project name. The project name can be accessed via ${ PROJECT_NAME},
# Since this is the top level CMakeLists.txt, the project name is also accessible
# with ${CMAKE_PROJECT_NAME} (both CMake variables are in-sync within the top level
# build script scope).
project("base_jni")

# 添加头文件的位置
include_directories(include)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.
#
# In this top level CMakeLists.txt, ${CMAKE_PROJECT_NAME} is used to define
# the target library name; in the sub-module's CMakeLists.txt, ${PROJECT_NAME}
# is preferred for the same purpose.
#
# In order to load a library into your app from Java/Kotlin, you must call
# System.loadLibrary() and pass the name of the library defined here;
# for GameActivity/NativeActivity derived applications, the same library name must be
# used in the AndroidManifest.xml file.
add_library(${CMAKE_PROJECT_NAME} SHARED
        # List C/C++ source files with relative paths to this CMakeLists.txt.
        base_test.cpp
        library_loader.cc
        TestData.cpp

        src/android/android_utils.cc
        src/android/build_info.cc
        src/base32/base32.cc
        src/base64/base64.cc
        src/jni/java_class.cpp
        src/jni/java_class_global_def.cpp
        src/jni/java_exception_thrower.cpp
        src/jni/java_global_ref_by_copy.cpp
        src/jni/java_global_ref_by_move.cpp
        src/jni/java_global_weak_ref.cpp
        src/jni/java_method.cpp
        src/jni/jni_util.cc
        src/jni/jni_utils.cpp
        src/jni/jni_weak_ref.cc
        src/jni/scoped_java_ref.cc
        src/log/log_settings.cc
        src/log/log_settings_state.cc
        src/log/logging.cc
        src/network/android_network_monitor.cc
        src/network/android_network_sharing_platform.cc
        src/network/connectivity_manager_impl.cc
        src/network/context_impl.cc
        src/network/jni_android_network_monitor.cc
        src/path/Path.cpp
        src/singleton/lazy_instance_helpers.cc
        src/stacktrace/stacktrace.cc
        src/string/string_conversion.cc
        src/thread/thread_local.cc
        src/thread/ThreadUtils.cpp
        src/utils/architecture.cpp
        src/utils/CountDownLatch.cpp
        src/utils/CyclicBarrier.cpp
        src/utils/unique_fd.cc)

# Specifies libraries CMake should link to your target library. You
# can link libraries from various origins, such as libraries defined in this
# build script, prebuilt third-party libraries, or Android system libraries.
target_link_libraries(${CMAKE_PROJECT_NAME}
        # List libraries link to the target library
        android
        log)