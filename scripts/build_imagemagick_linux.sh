#!/usr/bin/env bash

OS=linux
PROJECT=imagemagick
VERSION="6.8.9-8"
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
    cd "$SRC_DIR/$PROJECT-$VERSION"
    ./configure \
            --prefix="$BUILD_DIR" \
            --enable-shared=yes \
            --enable-static=yes \
            --with-quantum-depth=16 \
            --without-fftw \
            --without-dps \
            --without-lqr \
            --without-fpx \
            --without-lqr \
            --without-jbig \
            --without-jp2 \
            --without-openexr \
            --without-pango \
            --without-rsvg \
            --without-webp \
            --without-djvu \
            --without-windows-font-dir \
            --without-gs-font-dir \
            --without-x \
            --disable-openmp
}

function doBuild {
    # debug
    make clean
    CXXFLAGS="-g -O0" make -j8 install prefix="$BUILD_DIR/debug"

    # release
    make clean
    CXXFLAGS="-msse2 -O3" make -j8 install prefix="$BUILD_DIR/release"
}

function doCopy {
    mkdir -p "$TARGET_DIR/bin/linux/debug"
    mkdir -p "$TARGET_DIR/bin/linux/release"
    mkdir -p "$TARGET_DIR/include"
    cp -r "$BUILD_DIR/debug/lib"/* "$TARGET_DIR/bin/linux/debug"
    cp -r "$BUILD_DIR/release/lib"/* "$TARGET_DIR/bin/linux/release"
    cp -r "$BUILD_DIR/include/ImageMagick-6"/* "$TARGET_DIR/include"
}


doPrepare
doDownload
doUnzip
doConfigure
doBuild
doCopy