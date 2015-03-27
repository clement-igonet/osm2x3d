#ifndef OSM2X3D_MY3DWORLD_H
#define OSM2X3D_MY3DWORLD_H

#include "Singleton.h"
#include "My3DBuilding.h"
#include "My3DBuildingFloorPart.h"
#include "My3DGround.h"
#include <ostream>

class My3DWorld : public Singleton<My3DWorld> {
    friend class Singleton<My3DWorld>;
public:
    void addBuilding(shared_ptr<My3DBuilding> my3DBuilding);
    void buildX3Dom(int loD, bool hasTiles, ostream& outputStream);
//    int getNumberOfBuildings();
private:
    vector<shared_ptr<My3DBuilding >> buildings;
};

#endif // OSM2X3D_MY3DWORLD_H
