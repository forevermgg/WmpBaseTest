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