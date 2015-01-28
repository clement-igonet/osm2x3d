#include "My3DRoof.h"
#include <string>
using namespace std;

void My3DRoof::buildX3Dom(ostream& outputStream) {
}

My3DRoof::My3DRoof(
        shared_ptr <vector < pair<double, double >> > points,
        double elevation,
        Shape::Type shape,
        string colour) {
    this->points_ = points;
    this->elevation_ = elevation;
    this->shape_ = shape;
    this->colour_ = colour;
}
