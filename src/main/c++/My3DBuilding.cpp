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

void My3DBuilding::addBuildingPart(shared_ptr<My3DBuildingPart> my3DBuildingPart) {
    // Insert building part
    my3DBuildingParts_.push_back(my3DBuildingPart);
}

void My3DBuilding::addRoof(shared_ptr<My3DRoof> my3DRoof) {
    this->my3DBuildingRoofs.push_back(my3DRoof);
}

void My3DBuilding::buildX3Dom(ostream& outputStream) {











    for (
            vector<shared_ptr < My3DBuildingPart >> ::iterator my3DBuildingPartIt = this->my3DBuildingParts_.begin();
            my3DBuildingPartIt != this->my3DBuildingParts_.end();
            ++my3DBuildingPartIt) {

        double elevation = (*my3DBuildingPartIt)->elevation_;
        outputStream << R"(
                    <Transform translation='0 )" << elevation << " 0'>";
        outputStream << R"(
                        <Group>)";
        outputStream << R"(
                            <Shape>
                                <Appearance>)";
        string colour = (*my3DBuildingPartIt)->colour_;
        ostringstream diffuseColorSS;
        ostringstream transparencySS;
        double wallElevation;
        if (!(*my3DBuildingPartIt)->height_) {
            wallElevation = 9.99;
        } else {
            wallElevation = *((*my3DBuildingPartIt)->height_);
        }
        if (!colour.empty()) {
            diffuseColorSS << (*my3DBuildingPartIt)->colour_;
            transparencySS << (double) 0.8;
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
        shared_ptr < vector < pair<double, double >> > points = (*my3DBuildingPartIt)->points_;
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
                << "spine='0 0 0 0 " << wallElevation << " 0'/>";
        outputStream << R"(
                            </Shape>)";
        outputStream << R"(
                        </Group>
                    </Transform>)";
    }










    for (
            unordered_map<int, shared_ptr < My3DBuildingFloor >> ::iterator my3DBuildingFloorPairIt = this->my3DBuildingFloors.begin();
            my3DBuildingFloorPairIt != this->my3DBuildingFloors.end();
            ++my3DBuildingFloorPairIt) {
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
            double elevation = (*my3DBuildingFloorPartIt)->elevation_;
            outputStream << R"(
                    <Transform translation='0 )" << elevation << " 0'>";
            outputStream << R"(
                        <Group>)";
            outputStream << R"(
                            <Shape>
                                <Appearance>)";
            string colour = (*my3DBuildingFloorPartIt)->colour_;
            double wallElevation;
            if (!(*my3DBuildingFloorPartIt)->height_) {
                wallElevation = 9.99;
            } else {
                wallElevation = *((*my3DBuildingFloorPartIt)->height_);
            }
            outputStream << R"(
                                    <Material diffuseColor='1 1 1' transparency='0'/>)";
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
                    << "spine='0 0 0 0 0 0'/>";
            //                    << "spine='0 0 0 0 " << wallElevation << " 0'/>";
            outputStream << R"(
                            </Shape>)";
            outputStream << R"(
                        </Group>
                    </Transform>)";
        }
    }

    // Roof
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


