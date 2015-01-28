#ifndef OSM2X3D_MY3DBUILDINGFLOOR_H
#define OSM2X3D_MY3DBUILDINGFLOOR_H

#include "My3DBuildingFloorPart.h"
#include <vector>
#include <memory>
//#include <unordered_map>
using namespace std;

class My3DBuildingFloor {
    friend class My3DBuilding;
public:
    My3DBuildingFloor(int level);

private:
    int level_;
    shared_ptr<vector<shared_ptr<My3DBuildingFloorPart >> > my3DBuildingFloorParts_;
};

#endif // OSM2X3D_MY3DBUILDINGFLOOR_H