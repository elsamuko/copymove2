#!/usr/bin/env bash

# for gnu grep instead bsd grep
PATH=/opt/local/bin:"$PATH"

CLI="copymove2-cli"
APP="CopyMove2.app"

function indent {
    sed  's/^/     /'
}

function doChecks {
    echo "Checking"
    if [ ! -f "$CLI" ]; then
        echo "Missing $CLI"
        exit 1
    fi
    if [ ! -d "$APP" ]; then
        echo "Missing $APP"
        exit 1
    fi
}

function doInit {
    echo "Init"
    echo "    CLI : $CLI"
    echo "    APP : $APP"

    LIBS="$(otool -L "$CLI" | grep -Po "(?<=/opt/local/lib/)([^\s]+)")"
}

function copySelf {
    echo "Copying self into app"
    cp "$CLI" "$APP/Contents/Resources/$CLI"
}

function relinkLibs {
    echo "Relinking $INPUT"
    for LIB in $LIBS; do
        echo "    Relinking $LIB"
        install_name_tool -change "/opt/local/lib/$LIB" \
                "@loader_path/../Frameworks/$LIB" \
                "$APP/Contents/Resources/$CLI"
    done
}

doChecks
doInit
copySelf
relinkLibs
