#ifndef OSM2X3D_MY3DGROUNDTILE_H
#define OSM2X3D_MY3DGROUNDTILE_H
#include "Color.h"
#include <ostream>
#include <string>
using namespace std;

class My3DGroundTile {
public:
    My3DGroundTile(
            pair<double, double> minGroundTile,
            pair<double, double> maxGroundTile,
            int xTile, int yTile, int zoom);
    Color color_;
    pair<double, double> minGroundTile_;
    pair<double, double> maxGroundTile_;
    int xTile_;
    int yTile_;
    int zoom_;
};

#endif // OSM2X3D_MY3DGROUNDTILE_H
