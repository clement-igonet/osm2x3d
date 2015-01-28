#include "My3DBuilding.h"
#include <vector>
#include <memory>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cmath>
#include "Log.h"
using namespace std;

My3DBuilding::My3DBuilding(int id) : my3DBuildingFloors() {
    this->id_ = id;
}

/**
 * Get (create before if necessary) Floor from level field.
 * @param level
 * @return floor
 */
shared_ptr<My3DBuildingFloor> My3DBuilding::getFloor(int level) {
    unordered_map<int, shared_ptr < My3DBuildingFloor >> ::iterator my3DBuildingFloorIt = my3DBuildingFloors.find(level);
    shared_ptr<My3DBuildingFloor> my3DBuildingFloor;
    if (my3DBuildingFloorIt == my3DBuildingFloors.end()) {
        // Create a new floor
        my3DBuildingFloor = shared_ptr<My3DBuildingFloor > (new My3DBuildingFloor(level));
        std::pair<int, shared_ptr < My3DBuildingFloor >> my3DBuildingFloorEntry(level, my3DBuildingFloor);
        my3DBuildingFloors.insert(my3DBuildingFloorEntry);
    } else {
        my3DBuildingFloor = my3DBuildingFloorIt->second;
    }
    return my3DBuildingFloor;
}

//void My3DBuilding::setElevation(int level, double elevation) {
//    getFloor(level)->setElevation(elevation);
//}

void My3DBuilding::addLevel(int level, shared_ptr<My3DBuildingFloorPart> my3DBuildingFloorPart) {
    // Get list of floorParts
    shared_ptr < vector < shared_ptr < My3DBuildingFloorPart >> > my3DBuildingFloorParts = getFloor(level)->my3DBuildingFloorParts_;
    // Insert floor part
    my3DBuildingFloorParts->push_back(my3DBuildingFloorPart);
}

void My3DBuilding::addRoof(shared_ptr<My3DRoof> my3DRoof) {
    this->my3DBuildingRoofs.push_back(my3DRoof);
}

void My3DBuilding::buildX3Dom(ostream& outputStream) {
    for (
            unordered_map<int, shared_ptr < My3DBuildingFloor >> ::iterator my3DBuildingFloorPairIt = this->my3DBuildingFloors.begin();
            my3DBuildingFloorPairIt != this->my3DBuildingFloors.end();
            ++my3DBuildingFloorPairIt) {
        int level = my3DBuildingFloorPairIt->first;
        shared_ptr < vector < shared_ptr < My3DBuildingFloorPart >> > my3DBuildingFloorParts = my3DBuildingFloorPairIt->second->my3DBuildingFloorParts_;
        /**
         * 
        //      Y
        //     /|\
        //      |
        //      |
        //      |
        //      |
        //      |______\X
        //     /       /
        //    /
        //  |/_
        //  Z
        // Y translation for floor at level my3DBuildingFloorIt->level_;
         */
        for (
                vector < shared_ptr < My3DBuildingFloorPart >> ::iterator my3DBuildingFloorPartIt = my3DBuildingFloorParts->begin();
                my3DBuildingFloorPartIt != my3DBuildingFloorParts->end();
                ++my3DBuildingFloorPartIt) {
            //            double color = (double) (level % 100);
            //            double color = (double) ((17 * level) % 1000);
            double elevation = (*my3DBuildingFloorPartIt)->elevation_;
            //            outputStream << R"(
            //                    <Transform translation='0 )" << elevation << R"( 0'>)";
            //            int x = 150 * cos(((level * 10) * M_PI) / 180.0);
            //            int z = 150 * sin(((level * 10) * M_PI) / 180.0);
            outputStream << R"(
                    <Transform translation='0 )" << elevation << " 0'>";
            outputStream << R"(
                        <Group>)";
            //                     <Shape onmouseover="var myColor = document.getElementById('myX3d__myColor').getFieldValue('diffuseColor'); myColor.r = 0.5; document.getElementById('myX3d__myColor').setFieldValue('diffuseColor', myColor);"
            //                           onmouseout="var myColor = document.getElementById('myX3d__myColor').getFieldValue('diffuseColor'); myColor.r = 0.0; document.getElementById('myX3d__myColor').setFieldValue('diffuseColor', myColor);">
            //                        <Appearance>
            //                            <Material id="myColor" diffuseColor='0.961 0.961 0.863' transparency='0.2'/>
            //                        </Appearance>
            //                        <Extrusion convex='false' creaseAngle='0.785' crossSection='-1.87919 -20.7601 -15.3115 -2.6242 27.4095 15.5006 40.8419 -2.63532 -1.87919 -20.7601 ' solid='false' endCap='false' spine='0 0 0 0 330 0'/>
            //                    </Shape>
            outputStream << R"(
                            <Shape>
                                <Appearance>)";
            string colour = (*my3DBuildingFloorPartIt)->colour_;
            ostringstream diffuseColorSS;
            ostringstream transparencySS;
            double wallElevation;
            if (!(*my3DBuildingFloorPartIt)->height_) {
                wallElevation = 9.99;
            } else {
                wallElevation = *((*my3DBuildingFloorPartIt)->height_);
            }
            if (!colour.empty()) {
                diffuseColorSS << (*my3DBuildingFloorPartIt)->colour_;
                transparencySS << (double) 0.2;
            } else {
                diffuseColorSS << (double) (((13 * (level + 1 + (int) wallElevation)) % 100) / (double) 100) << " "
                        << (double) (((17 * (level + 1 + (int) wallElevation)) % 100) / (double) 100) << " "
                        << (double) (((23 * (level + 1 + (int) wallElevation)) % 100) / (double) 100);
                transparencySS << (double) 0.8;
                //            outputStream << R"(
                //                                    <TextureTransform scale=')"
                //                    //                    << "1 2'/>";
                //                    << (this->perimeter_ / 35.0)
                //                    << " " << (wallElevation / 24.0) << "'/>";
                //            //                        << " 1'/>";
                //            outputStream << R"(
                //                                    <ImageTexture url='ESB.png'/>)";
            }
            outputStream << R"(
                                    <Material diffuseColor=')" << diffuseColorSS.str();
            outputStream << R"(' transparency=')" << transparencySS.str() << "'/>";
            outputStream << R"(
                                </Appearance>)";
            shared_ptr < vector < pair<double, double >> > points = (*my3DBuildingFloorPartIt)->points_;
            stringstream pointsSS;
            for (
                    vector < pair<double, double >> ::iterator pointIt = points->begin();
                    pointIt != points->end();
                    ++pointIt) {
                pointsSS << pointIt->first << " ";
                pointsSS << pointIt->second << " ";
            }
            int level = my3DBuildingFloorPairIt->first;
            FILE_LOG(logDEBUG)
                    << "My3DBuilding::buildX3Dom - Level " << level
                    << ": elevation=" << elevation
                    << " / height=" << wallElevation;
            outputStream << R"(
                                <Extrusion convex='false' creaseAngle='0.785' crossSection=')" << pointsSS.str() << "' "
                    << "solid='false' endCap='false' "
                    << "spine='0 0 0 0 " << wallElevation << " 0'/>";
            outputStream << R"(
                            </Shape>)";
            outputStream << R"(
                        </Group>
                    </Transform>)";
        }
    }

    for (vector<shared_ptr < My3DRoof>>::iterator my3DRoofIt = my3DBuildingRoofs.begin();
            my3DRoofIt != my3DBuildingRoofs.end();
            ++my3DRoofIt) {
        //        (my3DRoofIt*)
        double roofElevation;
        if ((*my3DRoofIt)->elevation_ == 0) {
            roofElevation = 9.99;
        } else {
            roofElevation = (*my3DRoofIt)->elevation_;
        }
        outputStream << R"(
                    <Transform translation='0 )" << roofElevation << " 0'>";
        outputStream << R"(
                        <Group>)";
        outputStream << R"(
                            <Shape>
                                <Appearance>)";
        ostringstream diffuseColorSS;
        ostringstream transparencySS;

        diffuseColorSS << (*my3DRoofIt)->colour_;
        transparencySS << (double) 0.5;

        outputStream << R"(
                                    <Material diffuseColor=')" << diffuseColorSS.str();
        outputStream << R"(' transparency=')" << transparencySS.str() << "'/>";
        outputStream << R"(
                                </Appearance>)";
        shared_ptr < vector < pair<double, double >> > points = (*my3DRoofIt)->points_;
        stringstream pointsSS;
        for (
                vector < pair<double, double >> ::iterator pointIt = points->begin();
                pointIt != points->end();
                ++pointIt) {
            pointsSS << pointIt->first << " ";
            pointsSS << pointIt->second << " ";
        }
        outputStream << R"(
                                <Extrusion convex='false' creaseAngle='0.785' crossSection=')" << pointsSS.str() << "' "
                << "solid='false' endCap='false' "
                << "spine='0 0 0 0 0 0'/>";
        outputStream << R"(
                            </Shape>)";
        outputStream << R"(
                        </Group>
                    </Transform>)";
    }
}

string My3DBuilding::buildX3Dom() {
    ostringstream oss;
    this->buildX3Dom(oss);
    return oss.str();
}


