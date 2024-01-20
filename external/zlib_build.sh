#!/usr/bin/env bash
# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenColorIO Project.

set -ex

NDK=${1:-$NDK}

export ANDROID_NDK_HOME=$NDK

export ZLIB_MINIMUM_ANDROID_API=26

export WORKDIR=$(realpath ${WORKDIR:-$(pwd)})

export PREFIX=$WORKDIR/prefix/zlib

if [ -d $PREFIX ]; then
    echo "Target folder exists. Remove $PREFIX to rebuild"
    # exit 0
    rm -rf $PREFIX
fi

mkdir -p $PREFIX

ZLIB_VERSION="latest"

ZLIB_MAJOR_MINOR=$(echo "${ZLIB_VERSION}" | cut   -d. -f-2)
ZLIB_MAJOR=$(echo "${ZLIB_VERSION}" | cut   -d. -f-1)
ZLIB_MINOR=$(echo "${ZLIB_MAJOR_MINOR}" | cut   -d. -f2-)
ZLIB_PATCH=$(echo "${ZLIB_VERSION}" | cut   -d. -f3-)
ZLIB_VERSION_U="${ZLIB_MAJOR}.${ZLIB_MINOR}.${ZLIB_PATCH}"

if [ ! -d zlib ]; then
  git clone https://github.com/madler/zlib
  cd $WORKDIR/zlib

  if [ "$ZLIB_VERSION" == "latest" ]; then
    LATEST_TAG=$(git describe --abbrev=0 --tags)
    git checkout tags/${LATEST_TAG} -b ${LATEST_TAG}
  else
    git checkout tags/v${ZLIB_VERSION_U} -b v${ZLIB_VERSION_U}
  fi
fi

function build_zlib {
  mkdir -p $WORKDIR/zlib/build
  cd $WORKDIR/zlib
  cd $WORKDIR/zlib/build
  ABI=$1
  BUILD_ARCHS=$1
  cmake -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK_HOME/build/cmake/android.toolchain.cmake \
        -DCMAKE_BUILD_TYPE=Debug \
        -DANDROID_ABI=$1 \
        -DANDROID_NDK=$ANDROID_NDK_HOME \
        -DCMAKE_ANDROID_ARCH_ABI=$1 \
        -DCMAKE_ANDROID_NDK=$ANDROID_NDK_HOME \
        -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
        -DCMAKE_SYSTEM_NAME=Android \
        -DANDROID_NATIVE_API_LEVEL=$ZLIB_MINIMUM_ANDROID_API \
        -DCMAKE_INSTALL_PREFIX=$WORKDIR/prefix/zlib/$BUILD_ARCHS/lib \
        -DANDROID_CPP_FEATURES="rtti exceptions" \
        ../.

  cmake --build . \
        --target install \
        --config Debug \
        --parallel 2

  cd ../..
}


build_zlib armeabi-v7a
build_zlib arm64-v8a
build_zlib x86
build_zlib x86_64

rm -rf zlib