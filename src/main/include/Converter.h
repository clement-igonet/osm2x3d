#ifndef OSM2X3D_CONVERTER_H
#define OSM2X3D_CONVERTER_H

namespace Converter {
    const double RADIUS = 6371000.0; /* earth radius */
    void osmWorld23DWorld();
    void osmWorld23DGround();
    void osmWorld23DBuildings();
}

#endif // OSM2X3D_CONVERTER_H
