#include "Log.h"
#include "geometry/My3DGeometry.h"
#include <iostream>
#include <list>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>

pair<double, double > My3DGeometry::centroid(shared_ptr <vector < pair<double, double >> > points) {

    stringstream wktSS;
    bool firstTime = true;
    wktSS << "POLYGON((";
    for (
            vector < pair<double, double >> ::iterator pointIt = points->begin();
            pointIt != points->end();
            ++pointIt) {
        if (firstTime) {
            firstTime = false;
        } else {
            wktSS << ", ";
        }
        wktSS << pointIt->first << " " << pointIt->second << " ";
    }
    wktSS << "))";


    typedef boost::geometry::model::d2::point_xy<double> point_type;
    typedef boost::geometry::model::polygon<point_type> polygon_type;

    polygon_type poly;
    FILE_LOG(logINFO) << "My3DGeometry::centroid -wktSS : " << wktSS.str();
    //boost::geometry::read_wkt(
    //        "POLYGON((2 1.3,2.4 1.7,2.8 1.8,3.4 1.2,3.7 1.6,3.4 2,4.1 3,5.3 2.6,5.4 1.2,4.9 0.8,2.9 0.7,2 1.3)"
    //        "(4.0 2.0, 4.2 1.4, 4.8 1.9, 4.4 2.2, 4.0 2.0))", poly);
    boost::geometry::read_wkt(wktSS.str(), poly);

    point_type p;
    boost::geometry::centroid(poly, p);
    return make_pair(p.x(), p.y());
    //    std::cout << "centroid: " << boost::geometry::dsv(p) << std::endl;
}

