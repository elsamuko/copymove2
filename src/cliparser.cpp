#include <string>

#include "cliparser.hpp"
#include "tclap/CmdLine.h"
#include "log/log.hpp"

SorterParams cliparser::parseCLI( int argc, char** argv ) {

    SorterParams params;

    try {
        TCLAP::CmdLine cmd( "", ' ', "0.1" );

        TCLAP::UnlabeledValueArg<std::string> filename( "input", "Image to analyse", true, "", "filename" );
        cmd.add( filename );

        TCLAP::ValueArg<size_t> minimalHits( "m", "min", "Minimum of blocks needed to count as hit", false, 10, "number" );
        cmd.add( minimalHits );

        TCLAP::ValueArg<size_t> quality( "q", "quality", "Quality, the higher, the more similar two blocks have to be (1-10)", false, 5, "number" );
        cmd.add( quality );

        cmd.parse( argc, argv );

        params.setFilename( filename.getValue() );
        params.setMinimalHits( minimalHits.getValue() );
        params.setQuality( quality.getValue() );
        params.setValid( true );

    } catch( TCLAP::ArgException& e ) {
        std::stringstream message;
        message << e.error() << " for arg " << e.argId();
        LOG_ERROR( message.str() );
        params.setValid( false );
    }

    return params;
}
