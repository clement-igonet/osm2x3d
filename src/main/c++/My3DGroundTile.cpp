#include "My3DGroundTile.h"

My3DGroundTile::My3DGroundTile(
        pair<double, double> minGroundTile,
        pair<double, double> maxGroundTile,
        int xTile, int yTile, int zoom) {
    this->minGroundTile_ = minGroundTile;
    this->maxGroundTile_ = maxGroundTile;
    this->xTile_ = xTile;
    this->yTile_ = yTile;
    this->zoom_ = zoom;
}
