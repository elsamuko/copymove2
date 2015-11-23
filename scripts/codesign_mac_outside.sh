#!/usr/bin/env bash

if [ $# -lt 1 ]
then
    echo "Usage: $0 target.app"
    exit 1
fi

ARGV1="$1"

function indent {
    sed  's/^/    /'
}

function setVersion {
    echo "Include version and signer"
    local INCLUDE
    INCLUDE="$(dirname "$BASH_SOURCE")/version.sh"
    if [ ! -f "$INCLUDE" ]
    then
        echo "VERSION not set!"
        exit 1
    fi
    source "$INCLUDE"
}

function doInit {
    echo "Init"
    BASE=$(basename "$ARGV1")
    DIR=$(dirname "$ARGV1")
    NAME="${BASE%.app}"
    NAME_LOWER=$(echo "$NAME" | tr '[:upper:]' '[:lower:]')
    APP="$NAME.app"
    PLIST="$DIR/$APP/Contents/Info.plist"

    echo "    DIR      : $DIR"
    echo "    APP      : $APP"
}

function updatePlist {
    echo "Updating Info.plist"
    plutil -replace CFBundleIdentifier -string "com.fd-imaging.${NAME_LOWER}" "$PLIST"
    plutil -replace CFBundleShortVersionString -string "$VERSION" "$PLIST"
    plutil -replace LSApplicationCategoryType -string "public.app-category.photography" "$PLIST"
    plutil -replace LSMinimumSystemVersion -string "10.8.0" "$PLIST"
    # plutil -replace CFBundleIconFile -string "applet.icns" "$PLIST"
}

function signApp {
    echo "Signing $APP"
    codesign --deep --force --verify --verbose --sign "$CERT_ID_APP" "$DIR/$APP" 2>&1 | indent
}

function verifyApp {
    echo "Verifying"
    codesign -vvv -d "$DIR/$APP" 2>&1 | indent
}

setVersion
doInit
updatePlist
signApp
verifyApp
