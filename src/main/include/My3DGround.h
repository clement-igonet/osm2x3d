#ifndef OSM2X3D_MY3DGROUND_H
#define OSM2X3D_MY3DGROUND_H
#include "Singleton.h"
#include "My3DGroundTile.h"
#include <ostream>
#include <vector>
#include <memory>
using namespace std;

class My3DGround : public Singleton<My3DGround> {
    friend class Singleton<My3DGround>;
public:
    //    string buildX3Dom();
    void buildX3Dom(ostream& outputStream);
    pair<double, double> minGround_;
    pair<double, double> maxGround_;
    vector<shared_ptr<My3DGroundTile>> tiles_;
};

#endif // OSM2X3D_MY3DGROUND_H
