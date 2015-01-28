#ifndef OSM2X3D_OSM_NODE_H
#define OSM2X3D_OSM_NODE_H

class OsmWorld;

class Node {
    friend class OsmWorld;
    //    friend class Floor;
public:
    Node(int id, double lat, double lon);
    int id_;
    double lat_, lon_;
    //private:
};


#endif // OSM2X3D_OSM_NODE_H