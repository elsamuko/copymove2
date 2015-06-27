#!/usr/bin/env bash

OS=linux
PROJECT=imagemagick
VERSION="6.9.1-6"
DL_URL="http://mirror.checkdomain.de/imagemagick/releases/ImageMagick-$VERSION.zip"

SCRIPT_DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
MAIN_DIR="$SCRIPT_DIR/.."
TARGET_DIR="$MAIN_DIR/libs/$PROJECT"
PROJECT_DIR="$MAIN_DIR/tmp/$PROJECT"
BUILD_DIR="$MAIN_DIR/tmp/$PROJECT/build"
DOWNLOAD="$PROJECT_DIR/$PROJECT-$VERSION.zip"
SRC_DIR="$PROJECT_DIR/src"

function doPrepare {
    if [ -d "$SRC_DIR" ]; then
        rm -rf "$SRC_DIR"
    fi
    if [ -d "$TARGET_DIR" ]; then
        rm -rf "$TARGET_DIR"
    fi
    if [ -d "$BUILD_DIR" ]; then
        rm -rf "$BUILD_DIR"
    fi
    mkdir -p "$PROJECT_DIR"
    mkdir -p "$BUILD_DIR"
    mkdir -p "$TARGET_DIR"
    mkdir -p "$SRC_DIR"
}

function doDownload {
    if [ ! -f "$DOWNLOAD" ]; then
        curl "$DL_URL" -o "$DOWNLOAD"
    fi
}

function doUnzip {
    unzip -q -o "$DOWNLOAD" -d "$SRC_DIR"
    for FROM in "$SRC_DIR"/*; do
        echo $FROM
        mv "$FROM" "$SRC_DIR/$PROJECT-$VERSION"
        break
    done
}

function doConfigure {
    ./configure \
            --prefix="$BUILD_DIR" \
            --enable-shared=no \
            --enable-static=yes \
            --with-quantum-depth=16 \
            --without-fftw \
            --without-dps \
            --without-lqr \
            --without-fpx \
            --without-lqr \
            --without-jbig \
            --without-openjp2 \
            --without-openexr \
            --without-pango \
            --without-rsvg \
            --without-webp \
            --without-djvu \
            --without-windows-font-dir \
            --without-gs-font-dir \
            --without-x \
            --without-wmf \
            --without-gvc \
            --disable-openmp \
            --disable-docs
}

function doBuild {
    cd "$SRC_DIR/$PROJECT-$VERSION"

    # debug
    (export CXXFLAGS="$CXXFLAGS -g -O0"; \
    export CFLAGS="$CFLAGS -g -O0"; \
    doConfigure)
    make -j8 install prefix="$BUILD_DIR/debug"

    # release
    (export CXXFLAGS="$CXXFLAGS -msse2 -Ofast -finline -ffast-math -funsafe-math-optimizations"; \
    export CFLAGS="$CFLAGS -msse2 -Ofast -finline -ffast-math -funsafe-math-optimizations"; \
    doConfigure)
    make -j8 install prefix="$BUILD_DIR/release"
}

function doCopy {
    mkdir -p "$TARGET_DIR/bin/$OS/debug"
    mkdir -p "$TARGET_DIR/bin/$OS/release"
    mkdir -p "$TARGET_DIR/include"
    cp -r "$BUILD_DIR/debug/lib"/* "$TARGET_DIR/bin/$OS/debug"
    cp -r "$BUILD_DIR/release/lib"/* "$TARGET_DIR/bin/$OS/release"
    cp -r "$BUILD_DIR/include/ImageMagick-6"/* "$TARGET_DIR/include"
}


doPrepare
doDownload
doUnzip
doBuild
doCopy
