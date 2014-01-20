#ifndef IMAGE_H
#define IMAGE_H

#include <Magick++.h>

class Image {
public:
    Image();
    Image( int width, int height );
    Image( const std::string filename );

public:
    static void Initialize();
    static void Destroy();
    std::string toString();
    friend std::ostream& operator << ( std::ostream& stream, const Image& p );
    static bool initialized;

public:
    void getImage( void* data, bool hasAlpha, bool is16Bit );
    void setImage( void* data, bool hasAlpha, bool is16Bit );

    int  width() const;
    int  height() const;
    bool isNull() const;
    bool is16Bit() const;
    bool load( const std::string filename );
    bool save( const std::string filename, int quality = 95 );

    std::string colorSpace() const;
    std::string renderingIntent() const;
    std::string type() const;

private:
    /* the wrapped image */
    Magick::Image mImage;

    bool fileExists( const std::string& filename ) const;
};

#endif // IMAGE_H
