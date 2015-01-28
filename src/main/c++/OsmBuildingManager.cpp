#include "OsmBuildingManager.h"
#include "OsmBuilding.h"

#include <memory>
#include <utility>

using namespace std;

unordered_map<int, shared_ptr < OsmBuilding >> OsmBuildingManager::osmBuildings_;

shared_ptr<OsmBuilding> OsmBuildingManager::getBuilding(int id) {
    unordered_map<int, shared_ptr < OsmBuilding >> ::iterator osmBuildingIt =
            OsmBuildingManager::osmBuildings_.find(id);
    shared_ptr<OsmBuilding> osmBuilding;
    if (osmBuildingIt == OsmBuildingManager::osmBuildings_.end()) {
        // Create a new building
        osmBuilding = shared_ptr<OsmBuilding > (new OsmBuilding(id));
        std::pair<int, shared_ptr < OsmBuilding >> OsmBuildingEntry(id, osmBuilding);
        OsmBuildingManager::osmBuildings_.insert(OsmBuildingEntry);
    } else {
        osmBuilding = osmBuildingIt->second;
    }
    return osmBuilding;
}

shared_ptr<OsmBuilding> OsmBuildingManager::removeBuilding(int id) {
    unordered_map<int, shared_ptr < OsmBuilding >> ::iterator osmBuildingIt =
            OsmBuildingManager::osmBuildings_.find(id);
    shared_ptr<OsmBuilding> osmBuilding;
    if (osmBuildingIt != OsmBuildingManager::osmBuildings_.end()) {
        osmBuilding = osmBuildingIt->second;
        // Remove the existing building
        OsmBuildingManager::osmBuildings_.erase(osmBuildingIt);
    }
    return osmBuilding;
}
