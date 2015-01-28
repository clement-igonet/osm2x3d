#include "My3DWorld.h"

#include <ostream>

void My3DWorld::addBuilding(shared_ptr<My3DBuilding> my3DBuilding) {
    this->buildings.push_back(my3DBuilding);
}

void My3DWorld::buildX3Dom(ostream& outputStream) {
    //    outputStream << R"(<?xml version="1.0" encoding="UTF-8"?>
    outputStream << R"(<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE X3D PUBLIC "ISO//Web3D//DTD X3D 3.2//EN"
  "http://www.web3d.org/specifications/x3d-3.2.dtd">
<X3D profile="Interchange" version="3.2"
     xmlns:xsd="http://www.w3.org/2001/XMLSchema-instance"
     xsd:noNamespaceSchemaLocation="http://www.web3d.org/specifications/x3d-3.2.xsd">            
    <Scene>
        <navigationInfo id='nav' headlight='true' type='turntable' typeParams="0 0 0.2 1.4"></navigationInfo>
        <Background
            groundColor="0.972 0.835 0.666"
            skyAngle='1.309 1.571'
            skyColor='0.0 0.2 0.7 0.0 0.5 1.0 1.0 1.0 1.0'/>
        <Transform rotation='0 1 0 0' translation='6 3 6'>
            <Viewpoint description='Extruded room with a doorway' orientation='1 0 0 -1.57' position='0 500 0'/>
        </Transform>)";
    //    vector<shared_ptr<My3DBuilding >> buildings;
    for (vector < shared_ptr < My3DBuilding >> ::iterator buildingIt = buildings.begin();
            buildingIt != buildings.end();
            ++buildingIt) {

        //        outputStream << R"(
        //        <Group DEF=)";
        //        outputStream << "\'" << (*buildingIt)->name_ << "\'>";

        //        outputStream << R"(
        //        <Group onmouseover='alert(")";
        //        outputStream << (*buildingIt)->name_;
        //        outputStream << R"(")'>)";
        outputStream << R"(
        <Group
            DEF=")" << (*buildingIt)->name_ << "\" ";
        outputStream << R"(
            id=")" << (*buildingIt)->id_ << "\"";
        outputStream << R"(
            onclick="handleSingleClick(this);">)";
        //            onclick="handleGroupClick(event);">)";
        //        outputStream << "<Group>";
        (*buildingIt)->buildX3Dom(outputStream);
        outputStream << R"(
        </Group>)";
    }
    My3DGround::getInstance()->buildX3Dom(outputStream);
    outputStream << R"(
    </Scene>             
</X3D>)";
}
