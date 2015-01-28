#include "My3DGround.h"
#include <ostream>
using namespace std;
//
//                <Transform translation='0 0 0' rotation='1 0 0 4.7124'>
//                    <Group>
//                        <shape>
//                            <appearance>
//                                <imageTexture url='http://a.tile.openstreetmap.org/17/38598/49263.png'></imageTexture>
//                            </appearance>
//                            <Rectangle2D size='200 200' subdivision='1 1' ></Rectangle2D>
//                        </shape>
//                    </Group>
//                </Transform>
//

void My3DGround::buildX3Dom(ostream& outputStream) {
    for (
            vector < shared_ptr < My3DGroundTile >> ::iterator tileIt = this->tiles_.begin();
            tileIt != this->tiles_.end();
            ++tileIt) {
        double x0 = (*tileIt)->minGroundTile_.first;
        double z0 = (*tileIt)->minGroundTile_.second;
        double x1 = (*tileIt)->maxGroundTile_.first;
        double z1 = (*tileIt)->maxGroundTile_.second;
        outputStream << R"(
                    <Transform translation=')";
        outputStream << ((x1 + x0) / 2) << " " << -0.10 << " " << ((z1 + z0) / 2);
        //        0 0 0
        outputStream << R"(' rotation='1 0 0 -1.5708'>
                        <Group>
                            <Shape>
                                <Appearance>
                                    <ImageTexture url='"http://a.tile.openstreetmap.org/)";
        outputStream << (*tileIt)->zoom_ << "/" << (*tileIt)->xTile_ << "/" << (*tileIt)->yTile_;
        outputStream << R"(.png"'/>
                                </Appearance>
                                <Rectangle2D size=')";
        outputStream << (x1 - x0) << " " << (z1 - z0);
        outputStream << R"('></Rectangle2D>
                            </Shape>
                        </Group>
                    </Transform>)";
    }
}
//    My3DGround::getInstance()->minGround = make_pair(x0, z0);
//    My3DGround::getInstance()->maxGround = make_pair(x1, z1);
