
MAGICK_DIR=$${MAIN_DIR}/libs/imagemagick/bin/$${PLATFORM}/$${COMPILE_MODE}
DEFINES += 'MAGICKCORE_HDRI_ENABLE=0'
DEFINES += 'MAGICKCORE_QUANTUM_DEPTH=16'

INCLUDEPATH += $${MAIN_DIR}/libs/imagemagick/include
LIBS += -L$${MAGICK_DIR}

unix{
    LIBS += -L/usr/lib
    LIBS += $${MAGICK_DIR}/libMagick++-7.Q16.a \
            $${MAGICK_DIR}/libMagickWand-7.Q16.a \
            $${MAGICK_DIR}/libMagickCore-7.Q16.a \
            -llcms2 -ltiff -lfreetype -ljpeg -llzma \
            -lbz2 -lxml2 -lz -lm -lpthread -lltdl -lfontconfig
}
macx{
    INCLUDEPATH += /opt/local/include
    LIBS += -L/opt/local/lib
    LIBS += -lpng16
}
unix: !macx: LIBS += -lpng -lX11 -lXext

win32{
    DEFINES += STATIC_MAGICK
    LIBS += User32.lib Advapi32.lib OleAut32.lib Gdi32.lib
    LIBS += -lCORE_$${IM_FLAG}_coders_ -lCORE_$${IM_FLAG}_ffi_ -lCORE_$${IM_FLAG}_filters_ -lCORE_$${IM_FLAG}_jpeg_ \
            -lCORE_$${IM_FLAG}_lcms_ -lCORE_$${IM_FLAG}_libxml_ -lCORE_$${IM_FLAG}_magick_ -lCORE_$${IM_FLAG}_Magick++_ \
            -lCORE_$${IM_FLAG}_png_ -lCORE_$${IM_FLAG}_tiff_ -lCORE_$${IM_FLAG}_wand_ -lCORE_$${IM_FLAG}_zlib_
}

# set library search path to ./libImageMagick
# unix:!macx: QMAKE_LFLAGS += -Wl,--rpath=\\\$\$ORIGIN/libImageMagick

# build app package for mac
# macx: QMAKE_POST_LINK += $${MAIN_DIR}/scripts/fix_deployment_mac.sh $${DESTDIR}/$${TARGET}.app;
# macx: QMAKE_POST_LINK += macdeployqt $${DESTDIR}/$${TARGET}.$${TEMPLATE};

ioimage {
    HEADERS += $${SRC_DIR}/ioimage.hpp
    SOURCES += $${SRC_DIR}/ioimage.cpp
}
