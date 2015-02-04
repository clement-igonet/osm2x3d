#include "OsmWorld.h"
#include "My3DBuildingFloorPart.h"
#include "My3DBuilding.h"
#include "My3DWorld.h"
#include "OsmGround.h"
#include "OsmBuildingPart.h"
#include "OsmBuildingManager.h"
#include "OsmBuilding.h"
#include "osm/Node.h"
#include "osm/OsmUtil.h"
#include "Shape.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include "Log.h"
using boost::property_tree::ptree;
using boost::property_tree::ptree_error;
#include <unordered_map>
#include <utility>
#include <memory>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <limits>
#include <cmath>
using namespace std;

//#define PI           3.14159265358979323846  /* pi */

void OsmWorld::initFromStream(stringstream& osmXmlStream) {
    read_xml(osmXmlStream, pt);
    this->init_();
}

void OsmWorld::initFromFile(string osmXmlFile) {
    read_xml(osmXmlFile, pt);
    this->init_();
}

void OsmWorld::init_() {
    origin_ = pair<double, double> ();
    cout.precision(numeric_limits< double >::digits10);
    //    read_xml(osmXmlFile, pt);
    shared_ptr < unordered_map<long long, Node >> nodesMap(
            new unordered_map<long long, Node >);
    origin_.first = 0.0;
    origin_.second = 0.0;

    // Parse nodes

    BOOST_FOREACH(ptree::value_type const& v, pt.get_child("osm")) {
        if (v.first == "bounds") {
            //  <bounds minlat="40.7478800" minlon="-73.9870200" maxlat="40.7489700" maxlon="-73.9843000"/>
            this->minBound = make_pair(
                    v.second.get<double>("<xmlattr>.minlat"),
                    v.second.get<double>("<xmlattr>.minlon"));
            this->maxBound = make_pair(
                    v.second.get<double>("<xmlattr>.maxlat"),
                    v.second.get<double>("<xmlattr>.maxlon"));
            this->setOrigin(pair<double, double>(
                    minBound.first + (maxBound.first - minBound.first) / 2.0,
                    minBound.second + (maxBound.second - minBound.second) / 2.0));
            OsmGround::getInstance()->init(this->minBound, this->maxBound);
        } else if (v.first == "node") {
            long long id;
            try {
                id = v.second.get<long long>("<xmlattr>.id");
            } catch (const ptree_error &e) {
                string trace = v.second.get<string > ("<xmlattr>.id");
                FILE_LOG(logINFO)
                        << "node " << trace
                        << " / " << e.what();
            }
            double lat = v.second.get<double>("<xmlattr>.lat");
            double lon = v.second.get<double>("<xmlattr>.lon");
            Node node(id, lat, lon);
            nodesMap->insert(pair<long long, Node > (id, node));

        } else if (v.first == "way") {
            long long id;
            try {
                id = v.second.get<long long>("<xmlattr>.id");
            } catch (const ptree_error &e) {
                FILE_LOG(logINFO) << "way/" << e.what();
            }
            shared_ptr < vector < Node >> nodes(new vector < Node >);
            bool isBuilding = false;
            //            bool isBuildingPart = false;
            //            bool isBuildingPart = true;
            string name;
            string colour;
            double height = 0;
            double minHeight = 0;
            int buildingLevels = 1;
            int buildingMinLevel = 0;
            double roofHeight = 0;
            string roofShape("flat");

            BOOST_FOREACH(ptree::value_type const& w, v.second) {
                if (w.first == "nd") {
                    long long ref = w.second.get<long long> ("<xmlattr>.ref");
                    unordered_map<long long, Node >::iterator ndIt;
                    ndIt = nodesMap->find(ref);
                    if (ndIt == nodesMap->end()) {
                        // not found
                        cerr << "Cannot link way to this node: " << ref << endl;
                    } else {
                        nodes->push_back(ndIt->second);
                    }
                }
                if (w.first == "tag") {
                    if (w.second.get<string > ("<xmlattr>.k") == "building") {
                        isBuilding = true;
                        //                    } 
                        //                    else if (w.second.get<string > ("<xmlattr>.k") == "building:part") {
                        //                        isBuildingPart = true;
                    } else if (w.second.get<string > ("<xmlattr>.k") == "building:levels") {
                        try {
                            buildingLevels = w.second.get<int> ("<xmlattr>.v");
                        } catch (const ptree_error &e) {
                            string trace = w.second.get<string > ("<xmlattr>.v");
                            FILE_LOG(logINFO)
                                    << "building:levels - " << trace
                                    << " / " << e.what();
                            throw;
                        }
                        //                        FILE_LOG(logDEBUG) << "building:levels:" << buildingLevels << endl;
                    } else if (w.second.get<string > ("<xmlattr>.k") == "building:min_level") {
                        buildingMinLevel = w.second.get<int> ("<xmlattr>.v");
                    } else if (w.second.get<string > ("<xmlattr>.k") == "height") {
                        height = OsmUtil::height2m(
                                w.second.get<string > ("<xmlattr>.v"));
                    } else if (w.second.get<string > ("<xmlattr>.k") == "min_height") {
                        minHeight = OsmUtil::height2m(
                                w.second.get<string > ("<xmlattr>.v"));
                    } else if (w.second.get<string > ("<xmlattr>.k") == "name") {
                        name = w.second.get<string > ("<xmlattr>.v");
                    } else if (w.second.get<string > ("<xmlattr>.k") == "building:colour") {
                        colour = w.second.get<string > ("<xmlattr>.v");
                    }//<way id="53815257" visible="true" version="14" changeset="25664167" timestamp="2014-09-25T12:39:11Z" user="Goffredo" uid="194751">
                        //  <nd ref="679352088"/>
                        //  <nd ref="679352080"/>
                        //  <nd ref="679352072"/>
                        //  <nd ref="679352082"/>
                        //  <nd ref="679352096"/>
                        //  <nd ref="679352088"/>
                        //  <tag k="building" v="yes"/>
                        //  <tag k="building:material" v="glass"/>
                        //  <tag k="height" v="22 m"/>
                        //  <tag k="layer" v="1"/>
                        //  <tag k="name" v="Pyramide du Louvre"/>
                        //  <tag k="name:de" v="Pyramide im Louvre"/>
                        //  <tag k="name:et" v="Louvre'i püramiid"/>
                        //  <tag k="name:fr" v="Pyramide du Louvre"/>
                        //  <tag k="name:he" v="פירמידת מוזיאון הלובר"/>
                        //  <tag k="name:it" v="Piramide del Louvre"/>
                        //  <tag k="name:pl" v="Piramida Luwru"/>
                        //  <tag k="name:ru" v="Пирамида Лувра"/>
                        //  <tag k="name:uk" v="Піраміда Лувра"/>
                        //  <tag k="roof:height" v="22"/>
                        //  <tag k="roof:material" v="glass"/>
                        //  <tag k="roof:shape" v="pyramidal"/>
                        //  <tag k="source" v="cadastre-dgi-fr source : Direction Générale des Impôts - Cadastre. Mise à jour : 2009"/>
                        //  <tag k="wikipedia" v="fr:Pyramide du Louvre"/>
                        // </way>
                    else if (w.second.get<string > ("<xmlattr>.k") == "roof:shape") {
                        roofShape = w.second.get<string> ("<xmlattr>.v");
                    } else if (w.second.get<string > ("<xmlattr>.k") == "roof:height") {
                        roofHeight = OsmUtil::height2m(w.second.get<string > ("<xmlattr>.v"));
                    }
                }
            }
            // Roof for building part
            shared_ptr<OsmRoof> osmRoof(new OsmRoof(
                    nodes,
                    colour,
                    roofShape,
                    roofHeight));
            //            Shape::getEnumFromString(roofShape)

            // Process "way" as a floor object
            if (isBuilding) {
                FILE_LOG(logDEBUG)
                        << "OsmWorld::init() - buildingMinLevel: " << buildingMinLevel;
                shared_ptr<OsmBuildingPart> osmBuildingPart(new OsmBuildingPart(
                        nodes,
                        minHeight,
                        height,
                        buildingLevels,
                        buildingMinLevel,
                        name,
                        colour,
                        osmRoof));
                FILE_LOG(logINFO) << "OsmWorld::init - name: " << name;
                FILE_LOG(logINFO) << "OsmWorld::init - height: " << height;

                shared_ptr<OsmBuilding> osmBuilding(OsmBuildingManager::getBuilding(id));
                osmBuilding->osmBuildingParts_.push_back(osmBuildingPart);
                FILE_LOG(logINFO) << "OsmWorld::init - Building " << id
                        << "/Name: " << name;
                osmBuilding->name_ = name;
            } else
                //                if (isBuildingPart) 
            {
                //            if (isBuildingPart && minHeight == 0) {
                //                FILE_LOG(logDEBUG) << "minHeight:" << minHeight << endl;
                FILE_LOG(logDEBUG)
                        << "OsmWorld::init() - buildingMinLevel: " << buildingMinLevel;
                shared_ptr<OsmBuildingPart> osmBuildingPart(new OsmBuildingPart(
                        nodes,
                        minHeight,
                        height,
                        buildingLevels,
                        buildingMinLevel,
                        name,
                        colour,
                        osmRoof));
                //
                //                this->osmBuildingParts_.push_back(osmBuildingPart);
                this->osmBuildingParts_.insert(pair<long long, shared_ptr < OsmBuildingPart >> (id, osmBuildingPart));
            }
        } else if (v.first == "relation") {
            long long relationId = v.second.get<long long>("<xmlattr>.id");
            bool isBuilding = false;
            shared_ptr<OsmBuilding> osmBuilding;

            double height = 0;
            double minHeight = 0;
            string name;
            double roofHeight = 0;
            string roofShape("flat");

            BOOST_FOREACH(ptree::value_type const& w, v.second) {
                if (w.first == "member" && (w.second.get<string > ("<xmlattr>.type")) == "way") {
                    long long ref = w.second.get<long long> ("<xmlattr>.ref");
                    unordered_map<long long, shared_ptr < OsmBuildingPart >> ::iterator osmBuildingPartIt =
                            osmBuildingParts_.find(ref);
                    // if member is building part then add it in its building
                    if (osmBuildingPartIt != osmBuildingParts_.end()) {
                        //                        isBuilding = true;
                        osmBuilding = OsmBuildingManager::getBuilding(relationId);
                        osmBuilding->osmBuildingParts_.push_back(osmBuildingPartIt->second);
                    }
                } else if (w.first == "tag" && (w.second.get<string > ("<xmlattr>.k")) == "name") {
                    name = w.second.get<string> ("<xmlattr>.v");
                } else if (w.first == "tag" && (w.second.get<string > ("<xmlattr>.k")) == "building:part" && (w.second.get<string > ("<xmlattr>.v")) == "yes") {
                    isBuilding = true;
                } else if (w.first == "tag" && (w.second.get<string > ("<xmlattr>.k")) == "building" && (w.second.get<string > ("<xmlattr>.v")) == "yes") {
                    isBuilding = true;
                } else if (w.first == "tag" && (w.second.get<string > ("<xmlattr>.k")) == "type" && (w.second.get<string > ("<xmlattr>.v")) == "building") {
                    isBuilding = true;
                } else if (w.first == "tag" && (w.second.get<string > ("<xmlattr>.k")) == "height") {
                    height = OsmUtil::height2m(w.second.get<string > ("<xmlattr>.v"));
                } else if (w.first == "tag" && (w.second.get<string > ("<xmlattr>.k")) == "min_height") {
                    minHeight = OsmUtil::height2m(w.second.get<string > ("<xmlattr>.v"));
                } else if (w.first == "tag" && (w.second.get<string > ("<xmlattr>.k")) == "roof:shape") {
                    roofShape = w.second.get<string > ("<xmlattr>.v");
                } else if (w.first == "tag" && (w.second.get<string > ("<xmlattr>.k")) == "roof:height") {
                    roofHeight = OsmUtil::height2m(w.second.get<string > ("<xmlattr>.v"));
                }
            }
            if (!isBuilding) {
                //                osmBuilding = OsmBuildingManager::removeBuilding(relationId);
                OsmBuildingManager::removeBuilding(relationId);
            } else {
                osmBuilding = OsmBuildingManager::getBuilding(relationId);
                if (height != 0) {
                    FILE_LOG(logINFO) << "OsmWorld::init - relation height: " << height;
                    OsmBuildingManager::getBuilding(relationId)->name_ = name;
                    for (
                            vector < shared_ptr < OsmBuildingPart >> ::iterator osmBuildingPartIt = osmBuilding->osmBuildingParts_.begin();
                            osmBuildingPartIt != osmBuilding->osmBuildingParts_.end();
                            ++osmBuildingPartIt) {
                        shared_ptr < OsmBuildingPart > osmBuildingPart = (*osmBuildingPartIt);
                        FILE_LOG(logINFO) << "OsmWorld::init before - osmBuildingPart height: " << *(osmBuildingPart->maxHeight_);
                        if (!(osmBuildingPart->maxHeight_)) {
                            osmBuildingPart->maxHeight_ = height;
                        }
                        if (osmBuildingPart->minHeight_ == 0) {
                            osmBuildingPart->minHeight_ = minHeight;
                        }
                        FILE_LOG(logINFO) << "OsmWorld::init after  - osmBuildingPart height: " << *(osmBuildingPart->maxHeight_);
                    }
                }
            }
        }
    }

    FILE_LOG(logINFO) << "OsmWorld::init - " << OsmBuildingPart::titleString();
    for (auto it = osmBuildingParts_.begin(); it != osmBuildingParts_.end(); it++) {
        FILE_LOG(logINFO) << "OsmWorld::init - " << it->second->toString();
    }
    //unordered_map<long long, shared_ptr<OsmBuildingPart >>

    FILE_LOG(logINFO) << "OsmWorld::init - Statistic:";
    FILE_LOG(logINFO) << "Number of buildings: " << OsmBuildingManager::osmBuildings_.size();
    for (auto it = OsmBuildingManager::osmBuildings_.begin(); it != OsmBuildingManager::osmBuildings_.end(); it++) {
        FILE_LOG(logINFO)
                << "Building " << it->first << ": "
                << it->second->name_;
    }

    //    static unordered_map<int, shared_ptr < OsmBuilding >> osmBuildings_;

}

void OsmWorld::setOrigin(pair<double, double> origin) {
    this->origin_.first = origin.first;
    this->origin_.second = origin.second;
    FILE_LOG(logINFO)
            << "OsmWorld::setOrigin - lat/lon:" << origin.first
            << "/" << origin.second;
}
//
//void OsmWorld::processOrigin() {
//    origin_.first = 0;
//    origin_.second = 0;
//    vector<OsmBuildingPart>::iterator osmBuildingPartIt = osmBuildingParts_.begin();
//    if (osmBuildingPartIt != osmBuildingParts_.end()) {
//        shared_ptr < vector < Node> > nodes = osmBuildingPartIt->nodes_;
//        vector<Node> ::iterator nodeIt = nodes->begin();
//        if (nodeIt != nodes->end()) {
//            origin_.first = nodeIt->lat_;
//            origin_.second = nodeIt->lon_;
//        }
//    }
//}
