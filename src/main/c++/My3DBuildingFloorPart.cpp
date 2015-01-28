#include "My3DBuildingFloorPart.h"
#include <memory>
#include <iostream>

//My3DBuildingFloorPart::My3DBuildingFloorPart() {
//    this->init();
//}
//
//void My3DBuildingFloorPart::init() {
//    //    points_ = shared_ptr < vector < pair<double, double >> >(new vector < pair<double, double >>);
//}

My3DBuildingFloorPart::My3DBuildingFloorPart(
        shared_ptr <vector < pair<double, double >> > points,
        double height,
        double elevation,
        string colour) {
    //    this->init();
    this->points_ = points;
    if (height != 0) {
        this->height_ = height;
//        this->height_ = height;
    }
    this->elevation_ = elevation;
    this->colour_ = colour;
    //    FILE_LOG(logDEBUG) << "My3DBuildingFloorPart - height: " << height_ << endl;
}

