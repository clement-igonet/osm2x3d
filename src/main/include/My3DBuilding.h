#ifndef OSM2X3D_MY3DBUILDING_H
#define OSM2X3D_MY3DBUILDING_H

#include "My3DBuildingFloor.h"
#include "My3DBuildingPart.h"
#include <vector>
#include <memory>
#include <unordered_map>
using namespace std;

class OsmWorld;

class My3DBuilding {
    friend class OsmWorld;
public:
    My3DBuilding(int id);
    void addLevel(int level, shared_ptr<My3DBuildingFloorPart> my3DBuildingFloorPart);
    void addBuildingPart(shared_ptr<My3DBuildingPart> my3DBuildingPart);

    string buildX3Dom();
    void buildX3Dom(ostream& outputStream);
    string name_;
    int id_;
    double perimeter_;
    vector<shared_ptr<My3DBuildingPart >> my3DBuildingParts_;
private:
    shared_ptr<My3DBuildingFloor> getFloor(int level);
    unordered_map<int, shared_ptr<My3DBuildingFloor> > my3DBuildingFloors;
//    vector<shared_ptr<My3DRoof> > my3DBuildingRoofs;
};

#endif // OSM2X3D_MY3DBUILDING_H