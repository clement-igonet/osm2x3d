#ifndef OSM2X3D_MY3DROOF_H
#define OSM2X3D_MY3DROOF_H

#include "Shape.h"
#include <boost/assign/list_of.hpp>
#include <boost/unordered_map.hpp>
#include <iostream>
#include <utility>
#include <memory>
#include <vector>
#include <string>
using boost::assign::map_list_of;
using namespace std;

class My3DRoof {
public:

    My3DRoof(
            shared_ptr <vector < pair<double, double >> > points,
            double elevation,
            Shape::Type shape,
            string colour);
    void buildX3Dom(ostream& outputStream);
    shared_ptr <vector<pair<double, double >> > points_;
    string orientation_;
    double angle_;
    long levels_;
    double direction_;
    double elevation_;
    string colour_;
private:
    double height_;
    Shape::Type shape_;
};


#endif // OSM2X3D_MY3DROOF_H