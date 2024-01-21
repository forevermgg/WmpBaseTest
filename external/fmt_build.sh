#!/usr/bin/env bash

set -ex

NDK=$1
echo "NDK: $NDK"
# darwin-x86_64
HOST_PLATFORM=$2
echo "HOST_PLATFORM: $HOST_PLATFORM"
ANDROID_TARGET_API=$3
echo "ANDROID_TARGET_API: $ANDROID_TARGET_API"
export ANDROID_NDK_HOME=$NDK

TOOLCHAIN=${ANDROID_NDK_HOME}/toolchains/llvm/prebuilt/${HOST_PLATFORM}

export WORKDIR=$(realpath ${WORKDIR:-$(pwd)})

export PREFIX=$WORKDIR/prefix/fmt

if [ -d $PREFIX ]; then
    echo "Target folder exists. Remove $PREFIX to rebuild"
    # exit 0
    rm -rf $PREFIX
fi

mkdir -p $PREFIX

cd $WORKDIR
if [ ! -d fmt ]; then
    git clone --depth 10 https://github.com/fmtlib/fmt.git --branch 10.2.1
fi

cd fmt

echo "Building fmt in $(realpath $PWD), deploying to $PREFIX"

function build_fmt {
  cd $WORKDIR/fmt
  ANDROID_TARGET_ABI=$1

  build_directory="$PWD/build"
  if [ -d "build_directory" ]; then
    echo "build_directory $build_directory exists."
    cd build
  else
    echo "build_directory $build_directory does not exist."
    mkdir -p build && cd build
  fi

  cmake -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK_HOME/build/cmake/android.toolchain.cmake \
        -DCMAKE_BUILD_TYPE=Debug \
        -DANDROID_ABI=$1 \
        -DANDROID_NDK=$ANDROID_NDK_HOME \
        -DCMAKE_ANDROID_ARCH_ABI=$1 \
        -DCMAKE_ANDROID_NDK=$ANDROID_NDK_HOME \
        -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
        -DCMAKE_SYSTEM_NAME=Android \
        -DANDROID_NATIVE_API_LEVEL=${ANDROID_TARGET_API} \
        -DCMAKE_INSTALL_PREFIX=$WORKDIR/prefix/fmt/${ANDROID_TARGET_ABI} \
        -DANDROID_STL=c++_shared \
        ..

  cmake --build . \
        --target install \
        --config Debug \
        --parallel 2

  make && make install

  echo "Build OK fmt, PWD in $(realpath $PWD)"

  cd ../
  # 必须得移除一下build，不然会编译失败
  rm -rf build
  cd ../
}

build_fmt armeabi-v7a
build_fmt arm64-v8a
build_fmt x86
build_fmt x86_64
cd $WORKDIR/
rm -rf fmt