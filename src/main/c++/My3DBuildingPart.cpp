#include "My3DBuildingPart.h"
#include <memory>
#include <iostream>

My3DBuildingPart::My3DBuildingPart(
        shared_ptr <vector < pair<double, double >> > points,
//        double height,
        double elevation,
        string colour,
        shared_ptr<My3DRoof> my3DRoof) {
    //    this->init();
    this->points_ = points;
//    if (height != 0) {
//        this->height_ = height;
//    }
    this->elevation_ = elevation;
    this->colour_ = colour;
    this->my3DRoof_ = my3DRoof;
}

void My3DBuildingPart::buildX3Dom(ostream& outputStream) {
    outputStream << R"(
                    <Transform translation='0 )" << elevation_ << " 0'>";
    outputStream << R"(
                        <Group>)";
    outputStream << R"(
                            <Shape>
                                <Appearance>)";
    string colour = colour_;
    ostringstream diffuseColorSS;
    ostringstream transparencySS;
    double wallElevation;
    if (!height_) {
        wallElevation = 9.99;
    } else {
        wallElevation = *(height_);
    }
    FILE_LOG(logINFO) << "My3DBuildingPart::buildX3Dom - wallElevation: " << wallElevation;

    if (!colour.empty()) {
        diffuseColorSS << colour_;
        transparencySS << (double) 0.5;
    } else {
        diffuseColorSS << (double) (((13 * (1 + (int) wallElevation)) % 100) / (double) 100) << " "
                << (double) (((17 * (1 + (int) wallElevation)) % 100) / (double) 100) << " "
                << (double) (((23 * (1 + (int) wallElevation)) % 100) / (double) 100);
        transparencySS << (double) 0.8;
    }
    outputStream << R"(
                                    <Material diffuseColor=')" << diffuseColorSS.str();
    outputStream << R"(' transparency=')" << transparencySS.str() << "'/>";
    outputStream << R"(
                                </Appearance>)";
    stringstream pointsSS;
    for (
            vector < pair<double, double >> ::iterator pointIt = points_->begin();
            pointIt != points_->end();
            ++pointIt) {
        pointsSS << pointIt->first << " ";
        pointsSS << pointIt->second << " ";
    }
    outputStream << R"(
                                <Extrusion convex='false' creaseAngle='0.785' crossSection=')" << pointsSS.str() << "' "
            << "solid='false' endCap='false' "
            << "spine='0 0 0 0 " << wallElevation << " 0'/>";
    outputStream << R"(
                            </Shape>)";
    outputStream << R"(
                        </Group>
                    </Transform>)";

    // Building parts roof
    my3DRoof_->buildX3Dom(outputStream);
}
