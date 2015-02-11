#include "OsmBuildingPart.h"
#include <memory>
#include <iostream>
#include "Log.h"
#include <sstream>
using namespace std;

OsmBuildingPart::OsmBuildingPart(
        shared_ptr<vector<Node >> osmNodes,
        double osmMinHeight,
        int osmMinLevel,
        string name,
        string colour,
        shared_ptr<OsmRoof> osmRoof) {
    this->nodes_ = osmNodes;

    this->minHeight_ = osmMinHeight;
    this->minLevel_ = osmMinLevel;
    this->name_ = name;
    this->colour_ = colour;
    this->osmRoof_ = osmRoof;
}

string OsmBuildingPart::titleString() {
    stringstream ss;
    ss
            << "maxHeight     / "
            << "minHeight     / "
            << "levels        / "
            << "minLevel      / "
            << "nodes         / "
            << "name";
    return ss.str();
}

string OsmBuildingPart::toString() {
    stringstream ss;
    if (optHeight_) {
        ss << (*optHeight_) << "\t      / ";
    } else {
        ss << "X" << "\t      / ";
    }
    ss
            << minHeight_ << "\t      / "
            << levels_ << "\t      / "
            << minLevel_ << "\t      / "
            << nodes_->size() << "\t      / "
            << name_;

    return ss.str();
}
