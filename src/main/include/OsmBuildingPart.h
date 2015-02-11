#ifndef OSM2X3D_OSMBUILDINGPART_H
#define OSM2X3D_OSMBUILDINGPART_H

#include "Color.h"
#include "osm/Node.h"
#include "OsmRoof.h"
#include <utility>
#include <memory>
#include <vector>
#include <string>
#include <boost/optional.hpp>
using namespace std;

class OsmWorld;
class My3DBuilding;

class OsmBuildingPart {
    friend class OsmWorld;
    friend class My3DBuilding;
public:
    OsmBuildingPart(
            shared_ptr<vector<Node >> osmNodes,
            double osmMinHeight,
            int buildingMinLevel,
            string name,
            string colour,
            shared_ptr<OsmRoof> osmRoof_);
    static string titleString();
    string toString();

    shared_ptr<vector<Node> > nodes_;
    double minHeight_;
    boost::optional<double> optHeight_;
    boost::optional<int> levels_;
    int minLevel_;
    string name_;
    string colour_;
    shared_ptr<OsmRoof> osmRoof_;
    Color color_;
};

#endif // OSM2X3D_OSMBUILDINGPART_H