#ifndef OSM2X3D_OSMWORLD_H
#define OSM2X3D_OSMWORLD_H

#include "OsmBuilding.h"
#include "OsmBuildingManager.h"
#include "OsmBuildingPart.h"
#include "osm/Node.h"
#include "My3DBuilding.h"
#include "Singleton.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/optional.hpp>
using boost::property_tree::ptree;

#include <unordered_map>
#include <utility>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
using namespace std;

class OsmWorld : public Singleton<OsmWorld> {
    friend class Singleton<OsmWorld>;
public:
    void initFromFile(
            string osmXmlFile,
            boost::optional<int> zoom,
            boost::optional<int> xTile,
            boost::optional<int> yTile);
    void initFromStream(
            stringstream& osmXmlStream,
            boost::optional<int> zoom,
            boost::optional<int> xTile,
            boost::optional<int> yTile);
    void processOrigin();
    void process3DBuildings();
    void process3DGround();
    unordered_map<long long, shared_ptr<vector<long long >> > relations;
    pair<double, double> origin_;
private:
    void init_(
            boost::optional<int> zoom,
            boost::optional<int> height,
            boost::optional<int> width);
    ptree pt;
    int zoom;
    pair<double, double> minBound;
    pair<double, double> maxBound;
    shared_ptr<vector<long long >> getRelation(long long id);
    unordered_map<long long, shared_ptr<OsmBuildingPart >> osmBuildingParts_;
    void setOrigin(pair<double, double>);
};

#endif // OSM2X3D_OSMWORLD_H
