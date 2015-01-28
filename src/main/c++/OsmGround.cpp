#include "OsmGround.h"
#include "osm/OsmUtil.h"
#include <algorithm>
#include <iostream>
#include "Log.h"
using namespace std;

/**
 *                                    
 *              +--------->          
 *              |       X            
 *              |                    
 *              |                    
 * lat  ^       | Y                  
 *      |       v                    
 *      |                            
 *      |    A       B               
 *      |     +-----+    lat=lat_A   
 *      |     |     |                
 *      |     |     |    lon=lon_A   
 *      |     +-----+                
 *      |    D       C               
 *      |                            
 *      +----------------->          
 *                      long         
 *                                    
 * @param minBound
 * @param maxBound
 */
void OsmGround::init(
        pair<double, double> minBound,
        pair<double, double> maxBound) {
//    double phi = max(
//            maxBound.first - minBound.first,
//            maxBound.second - minBound.second);
//    // Take the integer part of decimal zoom
//    this->zoom = (int) (log2(360 / phi) - 0.5);
//    double lat = maxBound.first;
//    double lon = minBound.second;
//    this->xTile = OsmUtil::long2tilex(lon, zoom);
//    this->yTile = OsmUtil::lat2tiley(lat, zoom);
//    FILE_LOG(logINFO) << "OsmGround::init - zoom:" << zoom << endl;
//    FILE_LOG(logINFO) << "OsmGround::init - xTile:" << xTile << endl;
//    FILE_LOG(logINFO) << "OsmGround::init - yTile:" << yTile << endl;
    //    minGround = make_pair(
    //            OsmUtil::tiley2lat(yTile + 1, zoom),
    //            OsmUtil::tilex2long(xTile, zoom));
    //    maxGround = make_pair(
    //            OsmUtil::tiley2lat(yTile, zoom),
    //            OsmUtil::tilex2long(xTile + 1, zoom));
    minGround = make_pair(minBound.first, minBound.second);
    maxGround = make_pair(maxBound.first, maxBound.second);
    FILE_LOG(logINFO)
            << "OsmGround::init - minGround (lat/lon):" << minGround.first
            << "/" << minGround.second;
    FILE_LOG(logINFO)
            << "OsmGround::init - maxGround (lat/lon):" << maxGround.first
            << "/" << maxGround.second;
}
