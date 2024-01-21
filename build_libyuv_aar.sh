#!/usr/bin/env bash

set -ex

export WORKDIR=$(realpath ${WORKDIR:-$(pwd)})

cd $WORKDIR/third_party

if [ ! -d libyuv ]; then
    git clone https://github.com/lemenkov/libyuv.git
fi

export JAVA_HOME=/Library/Java/JavaVirtualMachines/jdk-17.0.4.1.jdk/Contents/Home

cd $WORKDIR/
./gradlew libyuv:assemble

