#ifndef OSM2X3D_OSMBUILDINGMANAGER_H
#define OSM2X3D_OSMBUILDINGMANAGER_H

#include "OsmBuilding.h"

#include <memory>
#include <unordered_map>

using namespace std;

class OsmBuildingManager {
public:
    static shared_ptr<OsmBuilding> getBuilding(int relationId);
    static shared_ptr<OsmBuilding> removeBuilding(int relationId);
    static unordered_map<int, shared_ptr < OsmBuilding >> osmBuildings_;
};

#endif // OSM2X3D_OSMBUILDINGMANAGER_H
