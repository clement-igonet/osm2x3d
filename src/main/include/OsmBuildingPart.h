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
    //    OsmBuildingPart();
    //    OsmBuildingPart(shared_ptr<vector<Node >> osmNodes, int osmHeight);
    //    OsmBuildingPart(
    //            shared_ptr<vector<Node >> osmNodes,
    //            int osmMinHeight,
    //            int osmHeight);
    OsmBuildingPart(
            shared_ptr<vector<Node >> osmNodes,
            double osmMinHeight,
//            double osmHeight,
            int buildingLevels,
            int buildingMinLevel,
            string name,
            string colour,
            shared_ptr<OsmRoof> osmRoof_);
    static string titleString();
    string toString();

    shared_ptr<vector<Node> > nodes_;
    //    void init();
    double minHeight_;
    boost::optional<double> optHeight_;
    //    double maxHeight_;
    int levels_;
    int minLevel_;
    string name_;
    string colour_;
    shared_ptr<OsmRoof> osmRoof_;
    Color color_;
    //private:
};

#endif // OSM2X3D_OSMBUILDINGPART_H