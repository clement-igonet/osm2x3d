#ifndef OSM2X3D_OSMGROUND_H
#define OSM2X3D_OSMGROUND_H

#include <utility>
#include "Singleton.h"

/**
 *            ^ Latitude          3D landmark       Tiles
 *            |
 *            + +90
 *            |
 *            |
 *            |
 *            |      Longitude               X                 X
 * --|--------+--------|-->       +----------->     +----------->
 *-180        |        +180*    / |                 |
 *            |               |/_ |                 |
 *            |             Y     |                 |
 *            |                   |  Z              |  Y
 *            + -90              \|/               \|/
 *            |
 */

class OsmGround : public Singleton<OsmGround> {
    friend class Singleton<OsmGround>;
public:
    void init(pair<double, double> minBound, pair<double, double> maxBound);
    pair<double, double> minGround;
    pair<double, double> maxGround;
    //private:
    //    int zoom;
    //    int xTile;
    //    int yTile;
};

#endif // OSM2X3D_OSMGROUND_H
