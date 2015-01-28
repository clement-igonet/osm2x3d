#ifndef OSM2X3D_OSMBUILDING_H
#define OSM2X3D_OSMBUILDING_H

#include "OsmBuildingPart.h"
//#include "OsmRoof.h"

#include <vector>
#include <memory>
#include <string>

using namespace std;

class OsmBuilding {
public:
    OsmBuilding(int id);
    vector<shared_ptr<OsmBuildingPart>> osmBuildingParts_;
//    shared_ptr<OsmRoof> osmRoof_;
    string name_;
    int getId();
private:
    int id;
};

#endif // OSM2X3D_OSMBUILDING_H
