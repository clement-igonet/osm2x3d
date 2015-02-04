#include "OsmBuildingPart.h"
#include <memory>
#include <iostream>
#include "Log.h"
#include <sstream>
using namespace std;
//OsmBuildingPart::OsmBuildingPart() {
//    this->init();
//}
//
//void OsmBuildingPart::init() {
//    this->minHeight_ = 0;
//    //    color_ = (Color)("black");
//    nodes_ = shared_ptr<vector<Node> > (new vector<Node>);
//}

OsmBuildingPart::OsmBuildingPart(
        shared_ptr<vector<Node >> osmNodes,
        double osmMinHeight,
        double osmHeight,
        int osmBuildingLevels,
        int osmMinLevel,
        string name,
        string colour,
        shared_ptr<OsmRoof> osmRoof) {
    this->nodes_ = osmNodes;
    this->maxHeight_ = osmHeight;
    this->minHeight_ = osmMinHeight;
    this->levels_ = osmBuildingLevels;
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
    ss
            << *maxHeight_ << "\t      / "
            << minHeight_ << "\t      / "
            << levels_ << "\t      / "
            << minLevel_ << "\t      / "
            << nodes_->size() << "\t      / "
            << name_;

    return ss.str();
}

//OsmBuildingPart::OsmBuildingPart(
//        shared_ptr<vector<Node >> osmNodes,
//        int osmMinHeight,
//        int osmHeight) {
//    OsmBuildingPart(osmNodes, 0, osmHeight, 0, 0);
//}
//
//OsmBuildingPart::OsmBuildingPart(shared_ptr<vector<Node >> osmNodes, int osmHeight) {
//    OsmBuildingPart(osmNodes, 0, osmHeight);
//}
//
