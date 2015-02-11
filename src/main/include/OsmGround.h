#ifndef OSM2X3D_OSMGROUND_H
#define OSM2X3D_OSMGROUND_H

#include <utility>
#include <boost/optional.hpp>
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
    void init(
            pair<double, double> minBound,
            pair<double, double> maxBound);
    void init(
            int zoom,
            int xTile,
            int yTile);
    pair<double, double> minGround;
    pair<double, double> maxGround;
    //private:
    boost::optional<int> zoom_;
    boost::optional<int> xTile_;
    boost::optional<int> yTile_;

};

#endif // OSM2X3D_OSMGROUND_H
