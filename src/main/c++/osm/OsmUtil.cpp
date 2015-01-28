#include "osm/OsmUtil.h"
#include <sstream>
#include <string>
#include <iostream>
#include <cmath>
using namespace std;

double OsmUtil::height2m(string height) {
    //    FILE_LOG(logDEBUG) << "height (string): " << height << endl;
    double result = 1;
    char unit = height[height.size() - 1];
    //    FILE_LOG(logDEBUG) << "unit: " << unit << endl;

    switch (unit) {
        case 'm':
        {
            stringstream buffer(height.substr(0, height.size() - 1));
            //            FILE_LOG(logDEBUG) << "buffer (meter): " << buffer.str() << endl;
            double value;
            buffer >> value;
            result = value;
        }
            break;
        default:
        {
            stringstream buffer(height);
            //            FILE_LOG(logDEBUG) << "buffer (no unit): " << buffer.str() << endl;
            double value;
            buffer >> value;
            result = value;
        }
            break;
    }
    //    FILE_LOG(logDEBUG) << "height (meter): " << result << endl;
    return result;
}

double OsmUtil::tilex2long(int x, int z) {
    return x / pow(2.0, z) * 360.0 - 180;
}

double OsmUtil::tiley2lat(int y, int z) {
    double n = M_PI - 2.0 * M_PI * y / pow(2.0, z);
    return 180.0 / M_PI * atan(0.5 * (exp(n) - exp(-n)));
}

int OsmUtil::long2tilex(double lon, int z) {
    return (int) (floor((lon + 180.0) / 360.0 * pow(2.0, z)));
}

int OsmUtil::lat2tiley(double lat, int z) {
    return (int) (floor((1.0 - log(tan(lat * M_PI / 180.0) + 1.0 / cos(lat * M_PI / 180.0)) / M_PI) / 2.0 * pow(2.0, z)));
}

string OsmUtil::osmColor2x3DColor(string colour) {
    string x3DColor;
    if (colour == "beige") { // #F5F5DC
        x3DColor = "0.961 0.961 0.863";
    }
    if (colour.substr(0, 1) == "#") {
        int redComp = std::stoi(colour.substr(1, 2), nullptr, 16);
        int greenComp = std::stoi(colour.substr(3, 2), nullptr, 16);
        int blueComp = std::stoi(colour.substr(5, 2), nullptr, 16);
        stringstream os;
        os << (float) redComp / (float) 255 << " " << (float) greenComp / (float) 255 << " " << (float) blueComp / (float) 255;
        x3DColor = os.str();
    }
    return x3DColor;
}
