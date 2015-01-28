#ifndef OSM2X3D_COLOR_H
#define OSM2X3D_COLOR_H

#include <string>
using namespace std;

class Color {
public:
    Color();
    explicit Color(string strColor);
    explicit Color(int color);
private:
    int color_;
};

#endif // OSM2X3D_COLOR_H