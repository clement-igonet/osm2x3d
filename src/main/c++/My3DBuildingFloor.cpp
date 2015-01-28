#include "My3DBuildingFloor.h"

#include <vector>
#include <memory>
using namespace std;

//My3DBuildingFloor::My3DBuildingFloor() {
//}
//

My3DBuildingFloor::My3DBuildingFloor(int level) :
level_(level),
my3DBuildingFloorParts_(new vector<shared_ptr<My3DBuildingFloorPart >>) {
    //    this->level_ = level;
    //    this->my3DBuildingFloorParts_ = my3DBuildingFloorParts;
}

//My3DBuildingFloor::My3DBuildingFloor(
//        int level,
//        shared_ptr<vector<shared_ptr<My3DBuildingFloorPart >> > my3DBuildingFloorParts) {
//    this->level_ = level;
//    this->my3DBuildingFloorParts_ = my3DBuildingFloorParts;
//}

//void My3DBuildingFloor::setElevation(double elevation) {
//    this->elevation_ = elevation;
//}
