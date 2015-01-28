#ifndef OSM2X3D_SHAPE_H
#define OSM2X3D_SHAPE_H

#include "Log.h"
#include <string>
#include <map>
#include <boost/unordered_map.hpp>
#include <boost/assign/list_of.hpp>

using namespace std;

class Shape {
public:

    /**
     * Types
     */
    enum Type {
        FLAT = 0,
        SKILLION = 1,
        GABLED = 2,
        HALF_HIPPED = 3,
        PYRAMIDAL = 4,
        GAMBREL = 5,
        MANSARD = 6,
        DOME = 7,
        ONION = 8,
        ROUND = 9,
        SALTBOX = 10
    };

    static int getEnumFromString(string strEnum) {
        int result;
        //        boost::unordered_map<std::string, int>::const_iterator pos = xmap.find(strEnum);
        boost::unordered_map<std::string, int>::const_iterator pos = xmap.find(strEnum);
        if (pos == xmap.end()) {
            return -1;
        } else {
            result = pos->second;
        }
        return result;
    }
private:
    static boost::unordered_map<std::string, int> xmap;
};

#endif // OSM2X3D_SHAPE_H
