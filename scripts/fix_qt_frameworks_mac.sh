#!/usr/bin/env bash

# reorder Qt frameworks according to
# https://developer.apple.com/library/mac/documentation/MacOSX/Conceptual/BPFrameworks/Concepts/FrameworkAnatomy.html
# call after macdeployqt

if [ $# -lt 1 ]; then
    echo "Usage: $(basename "$0") Program.app"
    exit 1
fi

APP_PATH="${1%/}"
DIR="$(dirname "$APP_PATH")"
APP="$(basename "$APP_PATH")"
NAME="${APP%.*}"
NAME_LOWER=$(echo "$NAME" | tr '[:upper:]' '[:lower:]')
PLIST="$APP_PATH/Contents/Info.plist"
QT_LOCAL="$(qmake -v | awk 'NR==2{print $6}')"
QT_LIBS=$(find "$APP_PATH" -name "Qt*" -type f -exec basename {} \;)


echo
echo "Fixing Qt frameworks"
echo "    APP  : [$APP]"
echo "    Qt   : [$QT_LOCAL]"
for QT_LIB in ${QT_LIBS[@]:0}; do
    echo
    echo "    Fixing $QT_LIB"
    echo "    Copying Info.plist to Resources/"
    FROM="$QT_LOCAL/$QT_LIB.framework/Contents/Info.plist"
    TO="$APP_PATH/Contents/Frameworks/$QT_LIB.framework/Resources/Info.plist"
	cp "$FROM" "$TO"

    # move Resources to /5/
    FROM="$APP_PATH/Contents/Frameworks/$QT_LIB.framework/Resources"
    TO="$APP_PATH/Contents/Frameworks/$QT_LIB.framework/Versions/5/Resources"
    echo "    Moving Resources/ to /5/"
	mv "$FROM" "$TO"

    # link Current to /5
    pushd . > /dev/null
    cd "$APP_PATH/Contents/Frameworks/$QT_LIB.framework/Versions"
    echo "    Linking Current to /5"
	ln -s "5" "Current"

    # link Resources to /5/Resources
    cd ..
    echo "    Linking Resources to /5/Resources"
	ln -s "Versions/Current/Resources" "Resources"

    # link lib to /Current/lib
	ln -s "Versions/Current/$QT_LIB" "$QT_LIB"
    echo "    Linking lib to /Current/lib"
	popd > /dev/null
done
