# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html.
# For more examples on how to use CMake, see https://github.com/android/ndk-samples.

# Sets the minimum CMake version required for this project.
cmake_minimum_required(VERSION 3.22.1)

# Declares the project name. The project name can be accessed via ${ PROJECT_NAME},
# Since this is the top level CMakeLists.txt, the project name is also accessible
# with ${CMAKE_PROJECT_NAME} (both CMake variables are in-sync within the top level
# build script scope).
project("flatbuffers")


set(FLATBUFFERS_SRC_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/../../../../third_party/flatbuffers/include)
message("FLATBUFFERS_SRC_INCLUDE ${FLATBUFFERS_SRC_INCLUDE}")
include_directories(${FLATBUFFERS_SRC_INCLUDE})

message("CMAKE_CURRENT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}")
set(FLATBUFFERS_SRC ${CMAKE_CURRENT_SOURCE_DIR}/../../../../third_party/flatbuffers)
message("FLATBUFFERS_SRC ${FLATBUFFERS_SRC}")

include_directories(${FLATBUFFERS_SRC}/include)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fexceptions -Wall -DFLATBUFFERS_TRACK_VERIFIER_BUFFER_SIZE")

# Certain platforms such as ARM do not use signed chars by default
# which causes issues with certain bounds checks.
set(CMAKE_CXX_FLAGS
        "${CMAKE_CXX_FLAGS} -fsigned-char")

set(FlatBuffers_Library_SRCS
        ${FLATBUFFERS_SRC}/include/flatbuffers/allocator.h
        ${FLATBUFFERS_SRC}/include/flatbuffers/array.h
        ${FLATBUFFERS_SRC}/include/flatbuffers/base.h
        ${FLATBUFFERS_SRC}/include/flatbuffers/buffer.h
        ${FLATBUFFERS_SRC}/include/flatbuffers/buffer_ref.h
        ${FLATBUFFERS_SRC}/include/flatbuffers/default_allocator.h
        ${FLATBUFFERS_SRC}/include/flatbuffers/detached_buffer.h
        ${FLATBUFFERS_SRC}/include/flatbuffers/flatbuffer_builder.h
        ${FLATBUFFERS_SRC}/include/flatbuffers/flatbuffers.h
        ${FLATBUFFERS_SRC}/include/flatbuffers/flexbuffers.h
        ${FLATBUFFERS_SRC}/include/flatbuffers/flex_flat_util.h
        ${FLATBUFFERS_SRC}/include/flatbuffers/hash.h
        ${FLATBUFFERS_SRC}/include/flatbuffers/idl.h
        ${FLATBUFFERS_SRC}/include/flatbuffers/minireflect.h
        ${FLATBUFFERS_SRC}/include/flatbuffers/reflection.h
        ${FLATBUFFERS_SRC}/include/flatbuffers/reflection_generated.h
        ${FLATBUFFERS_SRC}/include/flatbuffers/registry.h
        ${FLATBUFFERS_SRC}/include/flatbuffers/stl_emulation.h
        ${FLATBUFFERS_SRC}/include/flatbuffers/string.h
        ${FLATBUFFERS_SRC}/include/flatbuffers/struct.h
        ${FLATBUFFERS_SRC}/include/flatbuffers/table.h
        ${FLATBUFFERS_SRC}/include/flatbuffers/util.h
        ${FLATBUFFERS_SRC}/include/flatbuffers/vector.h
        ${FLATBUFFERS_SRC}/include/flatbuffers/vector_downward.h
        ${FLATBUFFERS_SRC}/include/flatbuffers/verifier.h
        ${FLATBUFFERS_SRC}/src/idl_parser.cpp
        ${FLATBUFFERS_SRC}/src/idl_gen_text.cpp
        ${FLATBUFFERS_SRC}/src/reflection.cpp
        ${FLATBUFFERS_SRC}/src/util.cpp
        ${FLATBUFFERS_SRC}/src/idl_gen_fbs.cpp
        ${FLATBUFFERS_SRC}/src/code_generators.cpp
        )

add_library( # Sets the name of the library.
        flatbuffers SHARED

        ${FlatBuffers_Library_SRCS}
        ${Generated_SRCS}
        )