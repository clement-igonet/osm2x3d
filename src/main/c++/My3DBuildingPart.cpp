#include "My3DBuildingPart.h"
#include <memory>
#include <iostream>

My3DBuildingPart::My3DBuildingPart(
        shared_ptr <vector < pair<double, double >> > points,
        double height,
        double elevation,
        string colour) {
    //    this->init();
    this->points_ = points;
    if (height != 0) {
        this->height_ = height;
    }
    this->elevation_ = elevation;
    this->colour_ = colour;
}

