#ifndef OSM2X3D_OSMROOF_H
#define OSM2X3D_OSMROOF_H

#include "Color.h"
#include "osm/Node.h"
#include "Shape.h"

#include <utility>
#include <memory>
#include <vector>
#include <string>
using namespace std;

class OsmRoof {
public:
    OsmRoof(
            shared_ptr<vector<Node >> osmNodes,
            string colour,
            string shape_,
            double height);
    string orientation_;
    double height_;
    double angle_;
    long levels_;
    double direction_;
    string colour_;
    bool isBuildingPart_;
    string shape_;

private:
    shared_ptr<vector<Node> > nodes_;
};

#endif // OSM2X3D_OSMBUILDING_H
