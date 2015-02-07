#ifndef GEOMETRY_MY_3D_GEOMETRY_H
#define GEOMETRY_MY_3D_GEOMETRY_H

#include <utility>
#include <memory>
#include <vector>
#include <string>
using namespace std;

namespace My3DGeometry {
    pair<double, double > centroid(shared_ptr <vector < pair<double, double >> > points);
}

#endif // GEOMETRY_MY_3D_GEOMETRY_H