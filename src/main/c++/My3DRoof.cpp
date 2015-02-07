#include "My3DRoof.h"
#include "geometry/My3DGeometry.h"
#include <string>
using namespace std;

My3DRoof::My3DRoof(
        shared_ptr <vector < pair<double, double >> > points,
        double elevation,
        //        double height,
        Shape::Type shape,
        string colour) {
    this->points_ = points;
    this->elevation_ = elevation;
    this->shape_ = shape;
    this->colour_ = colour;
    //    this->height_ = height;
}

void My3DRoof::buildX3Dom(ostream& outputStream) {

    FILE_LOG(logINFO) << "My3DRoof::buildX3Dom - elevation_: " << elevation_;
    //    FILE_LOG(logINFO) << "My3DRoof::buildX3Dom - height_: " << optHeight_ ? *optHeight_ : 0.0;

    stringstream roofPointsSS;
    for (
            vector < pair<double, double >> ::iterator pointIt = points_->begin();
            pointIt != points_->end();
            ++pointIt) {
        roofPointsSS << pointIt->first << " ";
        roofPointsSS << pointIt->second << " ";
    }

    double x, z;
    //    double xCenter, zCenter;
    pair<double, double > centerPoint;
    shared_ptr <vector<pair<double, double >> > centerPoints(new vector < pair<double, double >>);
    stringstream centerPointsSS;
    int it = 0;
    switch (shape_) {
        case Shape::PYRAMIDAL:
            // http://www.web3d.org/x3d/content/examples/Vrml2.0Sourcebook/Chapter15-Extrusion/_pages/page12.html
            centerPoint = My3DGeometry::centroid(points_);
            //            for (
            //                    vector < pair<double, double >> ::iterator pointIt = points_->begin();
            //                    pointIt != points_->end() && it < 4;
            //                    ++pointIt) {
            //                x += pointIt->first;
            //                z += pointIt->second;
            //                ++it;
            //            }
            //            xCenter = x / 4.0;
            //            zCenter = z / 4.0;
            for (
                    vector < pair<double, double >> ::iterator pointIt = points_->begin();
                    pointIt != points_->end();
                    ++pointIt) {
                centerPoints->push_back(make_pair(
                        pointIt->first - centerPoint.first,
                        pointIt->second - centerPoint.second));
            }
            for (
                    vector < pair<double, double >> ::iterator pointIt = centerPoints->begin();
                    pointIt != centerPoints->end();
                    ++pointIt) {
                centerPointsSS << pointIt->first << " ";
                centerPointsSS << pointIt->second << " ";
            }

            outputStream << R"(
                        <Transform translation=')" << centerPoint.first << " " << elevation_ << " " << centerPoint.second << "'>";

            //<Extrusion crossSection='-1 1 1 1 1 -1 -1 -1 -1 1' scale='1 1 0.01 0.01'/>
            //<!-- default Extrusion spine='0 0 0, 0 1 0' -->
            double height;
            if (optHeight_) {
                height = *optHeight_;
            } else {
                height = 9.99;
            }
            outputStream << R"(
                            <Group>
                                <Shape>
                                    <Extrusion crossSection=')" << centerPointsSS.str() << "' "
                    << "spine='0 0 0 0 " << height << " 0' "
                    << "scale='1 1 0.01 0.01'/>";
            //                    << "scale='1 1 1 1'/>";
            outputStream << R"(
                                        <Appearance>
                                        <Material diffuseColor='1 1 1' transparency='0.3'/>
                                    </Appearance>
                                </Shape>
                            </Group>
                        </Transform>)";
            break;
        case Shape::FLAT:
        default:
            double elevation;
            if (elevation_ == 0.0) {
                elevation = 9.99;
            }
            //            if (optElevation_) {
            //                elevation = *optElevation;
            //            } else {
            //                elevation = 9.99;
            //            }
            outputStream << R"(
                            <Transform translation='0 )" << elevation << " 0'>";
            outputStream << R"(
                                <Group>)";
            outputStream << R"(
                                    <Shape>
                                        <Appearance>)";
            ostringstream roofDiffuseColorSS;
            ostringstream roofTransparencySS;
            roofDiffuseColorSS << colour_;
            roofTransparencySS << (double) 0.5;
            outputStream << R"(
                                            <Material diffuseColor='0.835 0.82 0.784)";
            //                                        <Material diffuseColor=')" << diffuseColorSS.str();
            outputStream << R"(' transparency=')" << roofTransparencySS.str() << "'/>";
            outputStream << R"(
                                        </Appearance>)";
            outputStream << R"(
                                        <Extrusion convex='false' creaseAngle='0.785' crossSection=')" << roofPointsSS.str() << "' "
                    << "solid='false' endCap='false' "
                    << "spine='0 0 0 0 0 0'/>";
            outputStream << R"(
                                    </Shape>)";
            outputStream << R"(
                                </Group>
                            </Transform>)";
            break;
    }
    //                Shape::Type shape,

}