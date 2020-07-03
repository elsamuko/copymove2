#!/usr/bin/env bash

OS=win
PROJECT=imagemagick
VERSION="7.0.10-9"
DL_URL="https://www.imagemagick.org/download/windows/releases/ImageMagick-$VERSION.7z"

SCRIPT_DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
MAIN_DIR="$SCRIPT_DIR/.."
TARGET_DIR="$MAIN_DIR/libs/$PROJECT"
PROJECT_DIR="$MAIN_DIR/tmp/$PROJECT"
DOWNLOAD="$PROJECT_DIR/$PROJECT-$VERSION.7z"
SRC_DIR="$PROJECT_DIR/src"
BUILD_DIR="$SRC_DIR/$PROJECT-$VERSION"
BUILD_HELPER_DEBUG="$BUILD_DIR/VisualMagick/build_debug.bat"
BUILD_HELPER_RELEASE="$BUILD_DIR/VisualMagick/build_release.bat"

echo "SCRIPT_DIR  : $SCRIPT_DIR"
echo "MAIN_DIR    : $MAIN_DIR"
echo "TARGET_DIR  : $TARGET_DIR"
echo "PROJECT_DIR : $PROJECT_DIR"
echo "DOWNLOAD    : $DOWNLOAD"
echo "SRC_DIR     : $SRC_DIR"
echo "BUILD_DIR   : $BUILD_DIR"
echo

function indent {
    sed  's/^/     /'
}

function doPrepare {
    if [ -d "$SRC_DIR" ]; then
        rm -rf "$SRC_DIR"
    fi
    if [ -d "$TARGET_DIR" ]; then
        rm -rf "$TARGET_DIR"
    fi
    mkdir -p "$PROJECT_DIR"
    mkdir -p "$TARGET_DIR"
	mkdir -p "$TARGET_DIR/include/MagickCore"
	mkdir -p "$TARGET_DIR/include/MagickWand"
    mkdir -p "$SRC_DIR"
}

function doDownload {
    if [ ! -f "$DOWNLOAD" ]; then
        curl "$DL_URL" -o "$DOWNLOAD"
    fi
}

function doUnzip {
	7z x "$DOWNLOAD" -o"$SRC_DIR" -y
    for FROM in "$SRC_DIR"/*; do
        echo $FROM
        mv "$FROM" "$SRC_DIR/$PROJECT-$VERSION"
        break
    done
	
	# fix permissions
	find "$SRC_DIR/$PROJECT-$VERSION" -name *.exe -exec chmod +x {} \;
	find "$SRC_DIR/$PROJECT-$VERSION" -name *.bat -exec chmod +x {} \;
}

function create_helper {
	# VS 2015	
	echo -ne '@echo off\r\n\r\ncall "%VS140COMNTOOLS%..\\..\\VC\\bin\\vcvars32.bat"\r\n' > "$BUILD_HELPER_DEBUG"
	echo -ne '\r\n' >> "$BUILD_HELPER_DEBUG"
	echo -ne 'msbuild VisualStaticMT.sln /p:configuration=debug /p:platform=x64 /p:PlatformToolset=v140 /p:PreferredToolArchitecture=x64\r\n' >> "$BUILD_HELPER_DEBUG"
    chmod +x "$BUILD_HELPER_DEBUG"

	echo -ne '@echo off\r\n\r\ncall "%VS140COMNTOOLS%..\\..\\VC\\bin\\vcvars32.bat"\r\n' > "$BUILD_HELPER_RELEASE"
	echo -ne '\r\n' >> "$BUILD_HELPER_RELEASE"
	echo -ne 'msbuild VisualStaticMT.sln /p:configuration=release /p:platform=x64 /p:PlatformToolset=v140 /p:PreferredToolArchitecture=x64\r\n' >> "$BUILD_HELPER_RELEASE"
    chmod +x "$BUILD_HELPER_RELEASE"
}

function doBuild {
	create_helper
	
	# preconfigured project files
	cp -R "$SCRIPT_DIR/imagemagick_win"/* "$BUILD_DIR/"

    # debug and release
    cd "$BUILD_DIR/VisualMagick"
	rm -r lib
	cmd /c build_debug.bat
	mv lib lib_debug
	cmd /c build_release.bat
	mv lib lib_release
}

function doCopy {
    mkdir -p "$TARGET_DIR/bin/$OS/debug"
    mkdir -p "$TARGET_DIR/bin/$OS/release"
    mkdir -p "$TARGET_DIR/include"
    cp -r "$BUILD_DIR/VisualMagick/lib_debug"/* "$TARGET_DIR/bin/$OS/debug"
    cp -r "$BUILD_DIR/VisualMagick/lib_release"/* "$TARGET_DIR/bin/$OS/release"
    cp -r "$BUILD_DIR/ImageMagick/Magick++/lib/Magick++" "$TARGET_DIR/include"
	cp -r "$BUILD_DIR/ImageMagick/Magick++/lib/Magick++.h" "$TARGET_DIR/include"
	cp -r "$BUILD_DIR/ImageMagick/MagickCore"/*.h "$TARGET_DIR/include/MagickCore"
	cp -r "$BUILD_DIR/ImageMagick/MagickWand"/*.h "$TARGET_DIR/include/MagickWand"
}

echo "Prepare"
doPrepare | indent

echo "Download"
doDownload | indent

echo "Unzip"
doUnzip | indent

echo "Build"
doBuild 2>&1 | indent

echo "Copy"
doCopy | indent
