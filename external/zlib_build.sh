#!/usr/bin/env bash

set -ex

echo "第一个参数：$1"
echo "第二个参数：$2"
echo "第三个参数：$3"

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

  export AR=$TOOLCHAIN/bin/llvm-ar
  echo "AR: $AR"
  export CC=$TOOLCHAIN/bin/${TARGET}${ANDROID_TARGET_API}-clang
  echo "CC: $CC"
  export AS=$CC
  echo "AS: $AS"
  export CXX=$TOOLCHAIN/bin/${TARGET}${ANDROID_TARGET_API}-clang++
  echo "CXX: $CXX"
  export LD=$TOOLCHAIN/bin/ld
  echo "LD: $LD"
  export RANLIB=$TOOLCHAIN/bin/llvm-ranlib
  echo "RANLIB: $RANLIB"
  export STRIP=$TOOLCHAIN/bin/llvm-strip
  echo "STRIP: $STRIP"

  export ANDROID_PLATFORM=android-${ANDROID_TARGET_API}
  echo "ANDROID_PLATFORM: $ANDROID_PLATFORM"
  # Fix symbol 'gz_intmax' failed error
  # See https://github.com/madler/zlib/issues/856
  # sed -i '18d' zlib.map

  cmake -DCMAKE_TOOLCHAIN_FILE=${ANDROID_NDK_HOME}/build/cmake/android.toolchain.cmake \
        -DANDROID_ABI=$1 \
        -DCMAKE_BUILD_TYPE=Debug \
        -DANDROID_PLATFORM=${ANDROID_PLATFORM} \
        -DCMAKE_INSTALL_PREFIX=$WORKDIR/prefix/zlib/${ANDROID_TARGET_ABI} \
        ../.
  make
  make install

  echo "Build OK Zlib, PWD in $(realpath $PWD)"
  cd ../
  # 必须得移除一下build，不然会编译失败
  rm -rf build
  cd ../
}


build_zlib armeabi-v7a
build_zlib arm64-v8a
build_zlib x86
build_zlib x86_64

rm -rf zlib