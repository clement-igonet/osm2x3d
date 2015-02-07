#include "OsmRoof.h"
#include <string>
using namespace std;

OsmRoof::OsmRoof(
        shared_ptr<vector<Node >> nodes,
        string colour,
        string shape) {
    this->nodes_ = nodes;
    this->colour_ = colour;
    this->shape_ = shape;
//    this->height_ = height;
}
