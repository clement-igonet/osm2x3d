#ifndef OSM2X3D_OSM_OSMUTIL_H
#define OSM2X3D_OSM_OSMUTIL_H

#include <string>

using namespace std;

namespace OsmUtil {
    double height2m(string height);
    double tilex2long(int x, int z);
    double tiley2lat(int y, int z);
    int long2tilex(double lon, int z);
    int lat2tiley(double lat, int z);
    string osmColor2x3DColor(string colour);

}

#endif // OSM2X3D_OSM_OSMUTIL_H