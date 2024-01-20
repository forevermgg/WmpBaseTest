#!/bin/bash
set -u
set -e

NDK=${1:-$NDK}

export ANDROID_NDK_HOME=$NDK

export EXPAT_MINIMUM_ANDROID_API=26

export WORKDIR=$(realpath ${WORKDIR:-$(pwd)})

export PREFIX=$WORKDIR/prefix/expat

if [ -d $PREFIX ]; then
    echo "Target folder exists. Remove $PREFIX to rebuild"
    # exit 0
    rm -rf $PREFIX
fi

mkdir -p $PREFIX

cd $WORKDIR
if [ ! -d libexpat ]; then
    git clone --depth 10 https://github.com/libexpat/libexpat.git --branch R_2_5_0
fi

cd libexpat

echo "Building libexpat in $(realpath $PWD), deploying to $PREFIX"

function build_libexpat {
  cd expat
  ABI=$1
  BUILD_ARCHS=$1
  echo "Building libexpat for ${ABI}"

  echo "Building libexpat in $(realpath $PWD)"

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
        -DANDROID_NATIVE_API_LEVEL=$EXPAT_MINIMUM_ANDROID_API \
        -DCMAKE_INSTALL_PREFIX=$PREFIX/$1 \
        -DANDROID_STL=c++_shared \
        -DEXPAT_BUILD_DOCS=OFF \
        -DEXPAT_BUILD_EXAMPLES=OFF \
        -DEXPAT_BUILD_TESTS=OFF \
        -DEXPAT_BUILD_TOOLS=OFF \
        ..

  cmake --build . \
        --target install \
        --config Debug \
        --parallel 2

  make && make install

  echo "Build OK expat, PWD in $(realpath $PWD)"

  cd ../
  # 必须得移除一下build，不然会编译失败
  rm -rf build
  cd ../

  # mkdir -p $PREFIX/$BUILD_ARCHS/lib/
  # mkdir -p $PREFIX/$BUILD_ARCHS/include/
  # cp -p $WORKDIR/libexpat/expat/build/libexpat.so $WORKDIR/prefix/expat/$BUILD_ARCHS/lib/
}

build_libexpat armeabi-v7a
build_libexpat arm64-v8a
build_libexpat x86
build_libexpat x86_64

cd $WORKDIR
rm -rf libexpat