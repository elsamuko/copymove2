
MAGICK_DIR=$${MAIN_DIR}/libs/imagemagick/bin/$${PLATFORM}/$${COMPILE_MODE}
DEFINES += 'MAGICKCORE_HDRI_ENABLE=0'
DEFINES += 'MAGICKCORE_QUANTUM_DEPTH=16'

INCLUDEPATH += $${MAIN_DIR}/libs/imagemagick/include
LIBS += -L$${MAGICK_DIR}

unix {
    LIBS += $${MAGICK_DIR}/libMagick++-7.Q16.a \
            $${MAGICK_DIR}/libMagickWand-7.Q16.a \
            $${MAGICK_DIR}/libMagickCore-7.Q16.a \
            -lbz2 -ljpeg -ltiff
}

linux {
    LIBS += -L/usr/lib
    LIBS += -llcms2 -lfreetype -llzma \
            -lxml2 -lz -lm -lpthread -lltdl -lfontconfig \
            -lpng -lX11 -lXext
}

macx {
    # brew install libtiff libjpeg
    LIBS += -L/usr/local/lib
}

win32{
    DEFINES += STATIC_MAGICK
    LIBS += User32.lib Advapi32.lib OleAut32.lib Gdi32.lib
    LIBS += -lCORE_$${IM_FLAG}_coders_ -lCORE_$${IM_FLAG}_ffi_ -lCORE_$${IM_FLAG}_filters_ -lCORE_$${IM_FLAG}_jpeg_ \
            -lCORE_$${IM_FLAG}_lcms_ -lCORE_$${IM_FLAG}_libxml_ -lCORE_$${IM_FLAG}_MagickCore_ -lCORE_$${IM_FLAG}_Magick++_ \
            -lCORE_$${IM_FLAG}_png_ -lCORE_$${IM_FLAG}_tiff_ -lCORE_$${IM_FLAG}_MagickWand_ -lCORE_$${IM_FLAG}_zlib_
}

ioimage {
    HEADERS += $${SRC_DIR}/ioimage.hpp
    SOURCES += $${SRC_DIR}/ioimage.cpp
}
