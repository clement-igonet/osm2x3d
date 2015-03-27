#include "OsmWorld.h"
#include "Converter.h"
#include "My3DWorld.h"
#include <string>
#include <cstdio>
#include <cstdlib>
#include <getopt.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <boost/optional.hpp>

#include "Log.h"

using namespace boost;
using namespace std;

// http://doc.x3dom.org/tutorials/basics/imagesMovies/index.html
// http://wiki.openstreetmap.org/wiki/Slippy_map_tilenames
// http://www.turbosquid.com/FullPreview/Index.cfm/ID/525398

//int main(int argc, char* argv[]) {
//    OsmWorld *osmWorld = new OsmWorld("");
//    string x3dXml = osmWorld->buildX3D();
//}

/* Flag set by ‘--verbose’. */
static int verbose_flag = 0;
static int help_flag = 0;
static int has_tiles = 1;
//static int has_floors = 1;
//static int has_det_roof = 1;

/*******************************************************************************
 * This tool is a converter from Open Street Map xml data file
 * to X3D scene graph for buildings.
 * Usage: osm2x3d.uexe -i/--input INPUT -o/-output OUTPUT
 * Example:
 * $ osm2x3d.uexe -i esb.osm -o result.x3d --verbose
 * Options:
 * -i / --input   Open Street Map xml input file
 * -o / --output  X3D xml scene graph. Default is "result.x3d" on working
 * directory
 * -t / --notiles No OSM tile
 * --verbose/--brief     verbose/brief mode
 * -h/--help      print this help
 ******************************************************************************/
void usage() {
    cout << R"(
This tool is a converter from Open Street Map xml data file
to X3D scene graph for buildings.
Usage : osm2x3d.uexe - i / --input INPUT - o / -output OUTPUT
Example :
$ osm2x3d.uexe - i esb.osm - o result.x3d --verbose
Options :
-h / --help             print this help
--verbose / --brief     verbose / brief mode
-t / --noTile           no OSM tile
-i / --input            Open Street Map xml input file
-o / --output           X3D xml scene graph. Default is "result.x3d" on working
                        directory
-l / --LoD              Level of Detail (Building, BuildingPart)
-z / --zoom             zoom level (osm tile reference)
-x / --xTile            x tile
-y / --yTile            y tile)";
}
// cf http://www.boost.org/doc/libs/1_55_0/libs/log/example/doc/tutorial_file.cpp

int main(int argc, char **argv) {
    bool stdout = true;
    int loD = 4;
    int c;
    string inputFile("");
    string outputFile("result.x3d");
    boost::optional<int> zoom;
    boost::optional<int> xTile;
    boost::optional<int> yTile;

    Log::init();
    Log::setLevel(logINFO);

    //    FILE_LOG(logINFO) << "TestLog";

    //    using namespace log::trivial;
    //    log::sources::severity_logger< severity_level > lg;
    //
    //    BOOST_LOG_SEV(lg, trace) << "A trace severity message";
    //    BOOST_LOG_SEV(lg, debug) << "A debug severity message";
    //    BOOST_LOG_SEV(lg, info) << "An informational severity message";
    //    BOOST_LOG_SEV(lg, warning) << "A warning severity message";
    //    BOOST_LOG_SEV(lg, error) << "An error severity message";
    //    BOOST_LOG_SEV(lg, fatal) << "A fatal severity message";
    //    if (pLogSink) {
    //        pLogSink->flush();
    //    }
    while (1) {
        static struct option long_options[] = {
            /* These options set a flag. */
            {"brief", no_argument, &verbose_flag, 0},
            {"help", no_argument, &help_flag, 1},
            {"noTile", no_argument, &has_tiles, 0},
            {"verbose", no_argument, &verbose_flag, 1},
            /* These options don't set a flag.
               We distinguish them by their indices. */
            {"input", required_argument, 0, 'i'},
            {"output", required_argument, 0, 'o'},
            {"LoD", required_argument, 0, 'l'},
            {"xTile", required_argument, 0, 'x'},
            {"yTile", required_argument, 0, 'y'},
            {"zoom", required_argument, 0, 'z'},
            {0, 0, 0, 0}
        };
        /* getopt_long stores the option index here. */
        int option_index = 0;

        c = getopt_long(argc, argv, "bhi:o:l:tvx:y:z:", long_options, &option_index);

        /* Detect the end of the options. */
        if (c == -1)
            break;

        switch (c) {
            case 0:
                /* If this option set a flag, do nothing else now. */
                if (long_options[option_index].flag != 0)
                    break;
                FILE_LOG(logINFO)
                        << "option " << long_options[option_index].name
                        << (optarg ? " with arg " : "")
                        << (optarg ? optarg : "");
                break;

            case 'i':
                FILE_LOG(logINFO) << "option -i with value " << optarg;
                inputFile = optarg;
                break;

            case 'o':
                FILE_LOG(logINFO) << "option -o with value " << optarg;
                stdout = false;
                outputFile = optarg;
                break;

            case 'l':
                FILE_LOG(logINFO) << "option -l with value " << optarg;
                istringstream(optarg) >> loD;
                break;

            case 'x':
                FILE_LOG(logINFO) << "option -x with value " << optarg;
                istringstream(optarg) >> xTile;
                break;

            case 'y':
                FILE_LOG(logINFO) << "option -y with value " << optarg;
                istringstream(optarg) >> yTile;
                break;

            case 'z':
                FILE_LOG(logINFO) << "option -z with value " << optarg;
                istringstream(optarg) >> zoom;
                break;

            case '?':
                /* getopt_long already printed an error message. */
                break;

            default:
                abort();
        }
    }

    if (help_flag)
        usage();
    /* Instead of reporting ‘--verbose’
       and ‘--brief’ as they are encountered,
       we report the final status resulting from them. */
    if (verbose_flag)
        puts("verbose flag is set");

    /* Print any remaining command line arguments (not options). */
    if (optind < argc) {
        FILE_LOG(logINFO) << "non-option ARGV-elements: ";
        while (optind < argc) {
            FILE_LOG(logINFO) << argv[optind++];
        }
    }

    // Load OSM XML data to OSM objects
    if (inputFile.empty()) {
        stringstream ss;
        std::string line;
        while (std::getline(std::cin, line)) {
            ss << line << '\n';
        }
        OsmWorld::getInstance()->initFromStream(ss, zoom, xTile, yTile);
    } else {
        OsmWorld::getInstance()->initFromFile(inputFile, zoom, xTile, yTile);
    }
//    switch (loD) {
//        case 4:
//            has_floors = true;
//            has_det_roof = true;
//            break;
//        case 3:
//            has_floors = false;
//            has_det_roof = true;
//            break;
//        case 2:
//            has_floors = false;
//            has_det_roof = false;
//            break;
//        case 1:
//            has_floors = false;
//            has_det_roof = false;
//            break;
//        case 0:
//            has_floors = false;
//            has_det_roof = false;
//            break;
//    }

    // Convert OSM objects to My3D objects
    if (has_tiles) {
        Converter::osmWorld23DGround();
    }
    Converter::osmWorld23DBuildings();

    //    OsmWorld::getInstance()->process3DBuildings();
    //    OsmWorld::getInstance()->process3DGround();

    if (stdout) {
        // Extract X3D XML data from My3D objects
        My3DWorld::getInstance()->buildX3Dom(loD, has_tiles, *&cout);
    } else {
        std::shared_ptr<ofstream> outputStreamPtr = std::shared_ptr<ofstream> (new ofstream(outputFile));
        My3DWorld::getInstance()->buildX3Dom(loD, has_tiles, *outputStreamPtr);
        outputStreamPtr->close();
    }
    exit(EXIT_SUCCESS);
}
