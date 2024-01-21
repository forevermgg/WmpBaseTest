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

export PREFIX=$WORKDIR/prefix/jansson

if [ -d $PREFIX ]; then
    echo "Target folder exists. Remove $PREFIX to rebuild"
    # exit 0
    rm -rf $PREFIX
fi

mkdir -p $PREFIX

cd $WORKDIR
if [ ! -d jansson ]; then
    git clone --depth 10 https://github.com/akheron/jansson.git --branch v2.14
fi

cd jansson

echo "Building jansson in $(realpath $PWD), deploying to $PREFIX"

function build_jansson {
  cd $WORKDIR/jansson
  ANDROID_TARGET_ABI=$1

  if [ "$ANDROID_TARGET_ABI" == "armeabi-v7a" ]
  then
    export TARGET=armv7a-linux-androideabi

  elif [ "$ANDROID_TARGET_ABI" == "arm64-v8a" ]
  then
    export TARGET=aarch64-linux-android

  elif [ "$ANDROID_TARGET_ABI" == "x86" ]
  then
    export TARGET=i686-linux-android

  elif [ "$ANDROID_TARGET_ABI" == "x86_64" ]
  then
    export TARGET=x86_64-linux-android

  else
    echo "Unsupported target ABI: $ANDROID_TARGET_ABI"
    exit 1
  fi

  echo "TARGET:${TARGET}"

  mkdir -p build
  cd build
  cmake -DCMAKE_TOOLCHAIN_FILE=${ANDROID_NDK_HOME}/build/cmake/android.toolchain.cmake \
        -DANDROID_ABI=${ANDROID_TARGET_ABI} \
        -DANDROID_PLATFORM=${ANDROID_PLATFORM} \
        -DCMAKE_BUILD_TYPE=Debug \
        -DJANSSON_BUILD_SHARED_LIBS=ON \
        -DCMAKE_INSTALL_PREFIX=$WORKDIR/prefix/jansson/${ANDROID_TARGET_ABI} \
        -DANDROID_STL="c++_static" \
        -DCMAKE_SHARED_LINKER_FLAGS="-lm" \
        ../.
  make && make install
  echo "Build OK jansson, PWD in $(realpath $PWD)"
  cd ../
  # 必须得移除一下build，不然会编译失败
  rm -rf build
  cd ../
}

build_jansson armeabi-v7a
build_jansson arm64-v8a
build_jansson x86
build_jansson x86_64
rm -rf jansson
