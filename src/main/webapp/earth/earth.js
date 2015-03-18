
function handleSingleClick(Group) {
//    console.log("group", $(Group)[0]);
//    var id = $(Group)[0].getAttribute('id');
//    var def = $(Group)[0].getAttribute('DEF');
//    $('#info').html("<b>Info</b><br>"
//            + "id: " + id + "<br>"
//            + "DEF: " + def);
    document.getElementById('inline1').setAttribute('url', "./ball.x3d");
}

DISTANCE_LIMIT = 75000.0;
EARTH_RADIUS = 6372.7982;
EARTH_CIRC = EARTH_RADIUS * 2 * Math.PI;
TILE_SIZE = 256;
Osm2X3d.myZConst = 17;
var osm2X3d;
//var cameraTransform;
//var viewpointGround;
document.onload = function () {
//    var osm2X3d = new Osm2X3dEarth();
    osm2X3d = new Osm2X3dGround();
    osm2X3d.init();
    document.addEventListener('keydown', function (event) {
        if (event.keyCode == 88) { // 'x' keydown
            osm2X3d.updateView();
            osm2X3d.updateScene();
//                        element = document.getElementById('x3dElement');
//                        bindable = element.runtime.getActiveBindable('viewpoint');
//                        x3dom.debug.doLog('viewpoint.position: ' + bindable.getAttribute('position'));
        }
    }, false);
}

function Osm2X3dGround() {
    this.lat = 48.703885;
    this.lon = 2.0699095;
    this.elev = 14000;
    this.cameraTransform = document.createElement('Transform');
    this.viewpoint;
    this.camPos = new x3dom.fields.SFVec3f(0, this.elev, 0);
    this.camOri = -1.57;
}

Osm2X3dGround.prototype.init = function () {
    var self = this;
    var background = document.createElement('Background');
    background.setAttribute('groundColor', '0.972 0.835 0.666');
    background.setAttribute('skyAngle', '1.309 1.571');
    background.setAttribute('skyColor', '0.0 0.2 0.7 0.0 0.5 1.0 1.0 1.0 1.0');
    scene.appendChild(background);
    var navigationInfo = document.createElement('NavigationInfo');
    navigationInfo.setAttribute('id', 'nav');
    navigationInfo.setAttribute('headlight', 'true');
    navigationInfo.setAttribute('type', 'turntable');
    navigationInfo.setAttribute('typeParams', '0 0 0.0 1.56');
//                navigationInfo.setAttribute('visibilityLimit', '0');
    navigationInfo.setAttribute('transitionType', 'TELEPORT');
    scene.appendChild(navigationInfo);

    self.viewpoint = document.createElement('Viewpoint');
    self.viewpoint.setAttribute('id', 'viewpointGround');
    self.viewpoint.setAttribute('orientation', '1 0 0 -1.57');
    self.viewpoint.setAttribute('position', '0 ' + self.elev + ' 0');
    self.cameraTransform.setAttribute('rotation', '0 1 0 0');
    self.cameraTransform.appendChild(self.viewpoint);

    scene.appendChild(self.cameraTransform);
//    inline = document.createElement('inline');
//    inline.setAttribute('nameSpaceName', 'myX3d');
//    inline.setAttribute('url', './ground.x3d');
////    inline.setAttribute('url', './LasVegas.x3d');
//    scene.appendChild(inline);

//    x3dom.debug.doLog('xtile: ' + xtile, x3dom.debug.INFO);
//    x3dom.debug.doLog('ytile: ' + ytile, x3dom.debug.INFO);

//    updateGroundView(lat, lon, rdist);
//    updateGroundView();

    self.viewpoint.addEventListener("viewpointChanged", view_changed, false);
    self.updateView();
    self.updateScene();
}

Osm2X3dGround.prototype.updateView = function () {
    var self = this;
    var x3dElement = document.getElementById('x3dElement');
//    var scene = document.getElementById("scene");
    var curViewPoint = x3dElement.runtime.viewpoint();
    var rotationCenter = curViewPoint._vf.centerOfRotation;
    var orientation;
    var lonDiff = (180 / Math.PI) * rotationCenter.x / EARTH_RADIUS;
    var latDiff = -(180 / Math.PI) * rotationCenter.z / EARTH_RADIUS;
    self.lon += lonDiff;
    self.lat += latDiff;
    if (self.camOri[0] && self.camOri[1]) {
        orientation = (-self.camOri[0].x)
                + ' ' + (-self.camOri[0].y)
                + ' ' + (-self.camOri[0].z)
                + ' ' + (-self.camOri[1]);
    } else {
        orientation = self.viewpoint.getAttribute('orientation');
    }
    x3dom.debug.doLog('orientation: ' + orientation, x3dom.debug.INFO);
//    self.cameraTransform = document.createElement('Transform');
//    x3dom.debug.doLog('self.viewpoint : ' + self.viewpoint, x3dom.debug.INFO);
//    x3dom.debug.doLog('self.cameraTransform : ' + self.cameraTransform, x3dom.debug.INFO);
//    self.cameraTransform.removeChild(self.viewpoint);
//    var viewpoint = document.getElementById('viewpointGround');
    self.viewpoint.parentNode.removeChild(self.viewpoint)

    self.viewpoint = document.createElement('Viewpoint');
    self.viewpoint.setAttribute('id', 'viewpointGround');
    self.viewpoint.setAttribute('orientation', orientation);
    self.viewpoint.setAttribute('position',
            (self.camPos.x - rotationCenter.x)
            + ' ' + (self.camPos.y - rotationCenter.y)
            + ' ' + (self.camPos.z - rotationCenter.z));
    self.viewpoint.setAttribute('centerOfRotation', '0 0 0');
    self.cameraTransform.appendChild(self.viewpoint);
    self.viewpoint.setAttribute('set_bind', 'true');
    self.viewpoint.addEventListener("viewpointChanged", view_changed, false);
}

Osm2X3dGround.prototype.updateScene = function () {
    var self = this;
    zoom = Osm2X3d.processZoom(self.camPos);
    var xtile = Osm2X3d.long2xtile(self.lon, zoom);
    var ytile = Osm2X3d.lat2ytile(self.lat, zoom);
    var lonTile = Osm2X3d.xtile2long(xtile, zoom);
    var latTile = Osm2X3d.ytile2lat(ytile, zoom);
    var lonTilePlus = Osm2X3d.xtile2long(xtile + 1, zoom);
    var latTilePlus = Osm2X3d.ytile2lat(ytile + 1, zoom);
    var width = EARTH_RADIUS * (lonTilePlus - lonTile) * Math.PI / 180;
    var height = EARTH_RADIUS * (latTile - latTilePlus) * Math.PI / 180;
    var x = EARTH_RADIUS * (lonTile - self.lon) * Math.PI / 180;
    var z = EARTH_RADIUS * (self.lat - latTile) * Math.PI / 180;
    self.updateCoord(zoom);
//    var scene = document.getElementById("scene");
    var sceneContent = document.getElementById("x3dTile");
    if (sceneContent) {
        sceneContent.parentNode.removeChild(sceneContent);
    }
    var transform = document.createElement('Transform');
//    if (zoom <= 16) {
    var size = width + ' ' + height;
    var translation = (x + width / 2) + ' 0 ' + (z + height / 2);
//  <Transform id='x3dTile' translation="' + translation + '" rotation="1 0 0 -1.5708">
//      <Shape>
//          <Appearance>
//              <ImageTexture url=\'"' + url + '"\'/>
//          </Appearance>
//          <Rectangle2D size="' + size + '"></Rectangle2D>
//      </Shape>
//  </Transform>
    imageTexture = document.createElement('ImageTexture');
    imageTexture.setAttribute(
            'url',
            'http://a.tile.openstreetmap.org/'
            + zoom + '/' + xtile + '/' + ytile + '.png');
    appearance = document.createElement('Appearance');
    appearance.appendChild(imageTexture);
    rectangle = document.createElement('Rectangle2D');
    rectangle.setAttribute('size', size);
    shape = document.createElement('Shape');
    shape.appendChild(appearance);
    shape.appendChild(rectangle);
    transform.setAttribute('id', 'x3dTile');
    transform.setAttribute('translation', translation);
    transform.setAttribute('rotation', "1 0 0 -1.5708");
    transform.appendChild(shape);
    scene.appendChild(transform);

    if (zoom > 16) {
        
    }
//    } else {
//        inline = document.createElement('inline');
//        inline.setAttribute('id', 'x3dTile');
//        inline.setAttribute('nameSpaceName', 'myX3d');
////        var lonTile = xtile2long(xtile, zoom);
////        var latTile = ytile2lat(ytile, zoom);
////        var lonTilePlus = xtile2long(xtile + 1, zoom);
////        var latTilePlus = ytile2lat(ytile + 1, zoom);
////        inline.setAttribute('url',
////                'http://web.osm2x3d.net/osm2x3d.php?'
////                + 'll_lon=' + lonTile
////                + '&ll_lat=' + latTilePlus
////                + '&ur_lon=' + lonTilePlus
////                + '&ur_lat=' + latTile);
//        inline.setAttribute('url', 'LasVegas.x3d');
//        scene.appendChild(inline);
//    }
//    navigationInfo = document.createElement('NavigationInfo');
//    navigationInfo.setAttribute('id', 'nav');
//    navigationInfo.setAttribute('headlight', 'true');
//    navigationInfo.setAttribute('type', 'turntable');
//    navigationInfo.setAttribute('typeParams', '0 0 0.0 1.57');
//                navigationInfo.setAttribute('visibilityLimit', '0');
//                navigationInfo.setAttribute('transitionType', 'TELEPORT');
}

function view_changed(e) {
//    var self = this;

    osm2X3d.camPos = e.position;
    x3dom.debug.doLog('camPos: ' + osm2X3d.camPos, x3dom.debug.INFO);
    osm2X3d.camOri = e.orientation;
    x3dom.debug.doLog('camOri : ' + osm2X3d.camOri, x3dom.debug.INFO);

    x3dElement = document.getElementById('x3dElement');
    height = x3dElement.runtime.getHeight();
    x3dom.debug.doLog('height: ' + height, x3dom.debug.INFO);
    width = x3dElement.runtime.getWidth();
    x3dom.debug.doLog('width: ' + width, x3dom.debug.INFO);
    zoom = Osm2X3d.processZoom(osm2X3d.camPos);
    x3dom.debug.doLog('zoom: ' + zoom, x3dom.debug.INFO);
    // http://gis.stackexchange.com/questions/12991/how-to-calculate-distance-to-ground-of-all-18-osm-zoom-levels
    // http://wiki.openstreetmap.org/wiki/Zoom_levels
    // The distance represented by one pixel (S) is given by
    // S = C * cos(y) / 2 ^ (z + 8)
    // where...
    // C is the (equatorial) circumference of the Earth
    // z is the zoom level
    // y is the latitude of where you're interested in the scale. 

}

Osm2X3dGround.prototype.updateCoord = function (zoom) {
    var coordTrans = document.getElementById('coordTrans');
    if (coordTrans) {
        scene.removeChild(coordTrans);
    }

    coordinate = document.createElement('Coordinate');
    coordinate.setAttribute('id', 'coordinate');
    var fact = Math.pow(2, zoom);
    var zzz = '0 0 0 ' + 40000 / fact + ' 0 0 0 ' + 40000 / fact + ' 0 0 0 ' + 40000 / fact;
    x3dom.debug.doLog('zzz: ' + zzz, x3dom.debug.INFO);
    coordinate.setAttribute('point', zzz);
    color = document.createElement('Color');
    color.setAttribute('color', '1 0 0 0 1 0 0.2 0.2 1');
    indexedLineSet = document.createElement('IndexedLineSet');
    indexedLineSet.setAttribute('colorPerVertex', 'false');
    indexedLineSet.setAttribute('colorIndex', '0 1 2');
    indexedLineSet.setAttribute('coordIndex', '0 1 -1 0 2 -1 0 3 -1');
    indexedLineSet.setAttribute('solid', 'false');
    indexedLineSet.appendChild(coordinate);
    indexedLineSet.appendChild(color);
    shape = document.createElement('Shape');
    shape.appendChild(indexedLineSet);
    transformC = document.createElement('Transform');
    transformC.setAttribute('id', 'coordTrans');
    transformC.appendChild(shape);
    scene.appendChild(transformC);
}

function Osm2X3dEarth() {
}
Osm2X3dEarth.prototype.init = function () {
    var self = this;
    navigationInfo = document.createElement('NavigationInfo');
    navigationInfo.setAttribute('id', 'nav');
    navigationInfo.setAttribute('headlight', 'true');
    navigationInfo.setAttribute('type', 'turntable');
    navigationInfo.setAttribute('typeParams', '0 0 1.57 3.14');
    navigationInfo.setAttribute('visibilityLimit', '0');
    navigationInfo.setAttribute('transitionType', 'TELEPORT');
    scene.appendChild(navigationInfo);
    self.viewpoint = document.createElement('Viewpoint');
    self.viewpoint.setAttribute('id', 'viewpointEarth');
    viewpoint.setAttribute('position', '0 0 300000');
    scene.appendChild(viewpoint);
    inline = document.createElement('inline');
    inline.setAttribute('nameSpaceName', 'myX3d');
    inline.setAttribute('url', './earth.x3d');
    scene.appendChild(inline);
    viewpoint.addEventListener("viewpointChanged", view_earth_changed, false);
}

Osm2X3dEarth.prototype.view_changed = function (e) {
//                var NTInfo;
//                var infoPane;
//                NTInfo = x3dom.docs.getNodeTreeInfo();
//                infoPane = document.getElementById('info');
//                infoPane.innerHTML = NTInfo;
//                if (reloadNeeded) {
//                    reloadNeeded = false;
//                    x3dom.reload();
//                    document.getElementById('x3dElement').setAttribute('showlog', 'true');
//                }
    var pos = e.position;
    x3dom.debug.doLog('viewpoint position: ' + pos, x3dom.debug.INFO);
    rdist2 = Math.pow(pos.x, 2) + Math.pow(pos.y, 2) + Math.pow(pos.z, 2);
    rdist = Math.sqrt(rdist2);
    x3dom.debug.doLog('radial distance: ' + rdist, x3dom.debug.INFO);
//                myX3dMode = "ground";
    self.lon = Math.atan(pos.x / pos.z) * 180.0 / Math.PI;
    self.lat = Math.acos(pos.y / rdist) * 180.0 / Math.PI;
    x3dom.debug.doLog('lon: ' + self.lon, x3dom.debug.INFO);
    x3dom.debug.doLog('lat: ' + self.lat, x3dom.debug.INFO);
//                document.getElementById('inlineEarth').setAttribute('render', "false");
//
//                inlineGround = document.createElement('inline');
//                inlineGround.setAttribute('id', 'myInlineGroundId');
//                inlineGround.setAttribute('nameSpaceName', 'myInlineGround');
//                inlineGround.setAttribute('url', './ground.x3d');
//                inlineGround.setAttribute('load', 'true');
//                scene.appendChild(inlineGround);
//
//                viewpointGround = document.getElementById('viewpointGround');
//                
//                    position = viewpointGround.getAttribute('position');
//                    position.x = 0;
//                    position.y = 0;
//                    position.z = rdist - EARTH_RADIUS;
//                viewpointGround.setAttribute('position', '0 0 ' + (rdist - EARTH_RADIUS));
//                viewpointGround.setAttribute('set_bind', 'true');
}

function Osm2X3d() {
}

Osm2X3d.long2xtile = function (lon, zoom) {
    return (Math.floor((lon + 180) / 360 * Math.pow(2, zoom)));
}
Osm2X3d.lat2ytile = function (lat, zoom) {
    return (Math.floor((1 - Math.log(Math.tan(lat * Math.PI / 180) + 1 / Math.cos(lat * Math.PI / 180)) / Math.PI) / 2 * Math.pow(2, zoom)));
}
Osm2X3d.xtile2long = function (x, z) {
    return (x / Math.pow(2, z) * 360 - 180);
}
Osm2X3d.ytile2lat = function (y, z) {
    var n = Math.PI - 2 * Math.PI * y / Math.pow(2, z);
    return (180 / Math.PI * Math.atan(0.5 * (Math.exp(n) - Math.exp(-n))));
}
Osm2X3d.processDist = function (camCoord) {
    var rdist2 = Math.pow(camCoord.x, 2)
            + Math.pow(camCoord.y, 2)
            + Math.pow(camCoord.z, 2);
    return Math.sqrt(rdist2);
}
Osm2X3d.processZoom = function (camCoord) {
    var rdist2 = Math.pow(camCoord.x, 2)
            + Math.pow(camCoord.y, 2)
            + Math.pow(camCoord.z, 2);
    var rdist = Math.sqrt(rdist2);
    var zoom = parseInt(Osm2X3d.myZConst - Math.log2(rdist));
    var zoom = Math.min(zoom, 19);
    var zoom = Math.max(zoom, 1);
    return zoom;
}

