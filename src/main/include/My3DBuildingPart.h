#ifndef OSM2X3D_MY3DBUILDINGPART_H
#define OSM2X3D_MY3DBUILDINGPART_H

#include "Color.h"
#include "osm/Node.h"
#include "My3DRoof.h"

#include <utility>
#include <memory>
#include <vector>
#include <string>
#include <boost/optional.hpp>

using namespace std;

class OsmWorld;
class My3DBuilding;

class My3DBuildingPart {
    friend class OsmWorld;
    friend class My3DBuilding;
public:
    My3DBuildingPart(
            shared_ptr <vector < pair<double, double >> > points,
//            double height,
            double elevation,
            string colour,
            shared_ptr<My3DRoof> my3DRoof);
    void buildX3Dom(ostream& outputStream);
    shared_ptr<vector<pair<double, double >> > points_;
    shared_ptr<My3DRoof> my3DRoof_;
    boost::optional<double> optHeight_;
private:
    double elevation_;
    string colour_;
};

#endif // OSM2X3D_MY3DBUILDINGPART_H
