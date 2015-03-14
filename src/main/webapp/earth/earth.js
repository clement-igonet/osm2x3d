
function handleSingleClick(Group) {
//    console.log("group", $(Group)[0]);
//    var id = $(Group)[0].getAttribute('id');
//    var def = $(Group)[0].getAttribute('DEF');
//    $('#info').html("<b>Info</b><br>"
//            + "id: " + id + "<br>"
//            + "DEF: " + def);
    document.getElementById('inline1').setAttribute('url', "./ball.x3d");

}

//            reloadNeeded = false;
EARTH_MODE = 0;
GROUND_MODE = 1;
//            myX3dMode = EARTH_MODE;
myX3dMode = GROUND_MODE;
DISTANCE_LIMIT = 75000.0;
EARTH_RADIUS = 6372.7982;
EARTH_CIRC = EARTH_RADIUS * 2 * Math.PI;
focus = 0.153;
TILE_SIZE = 256;
myZConst = 17;
//var lon = 0;
//var lat = Math.PI / 2;
var lat, lon, elev, zoom;
lat = 48.703885;
lon = 2.0699095;
elev = 14000;
var cameraTransform;
var viewpointGround;
var grCamPos = new x3dom.fields.SFVec3f(0, elev, 0);
var grCamOri = -1.57;

document.onload = function () {
    switch (myX3dMode) {
        case EARTH_MODE:
            initEarthMode();
            break;
        case GROUND_MODE:
            initGroundMode(lat, lon, elev);
            break;
        default:
    }

    document.addEventListener('keydown', function (event) {
        // 'x' keydown
        if (event.keyCode == 88) {
//            updateGroundView(41.632653061224495, 3.214285714285846, 14000);
            updateGroundView();
//                        element = document.getElementById('x3dElement');
//                        bindable = element.runtime.getActiveBindable('viewpoint');
//                        x3dom.debug.doLog('viewpoint.position: ' + bindable.getAttribute('position'));
        }
    }, false);
}

function initGroundMode() {

    background = document.createElement('Background');
    background.setAttribute('groundColor', '0.972 0.835 0.666');
    background.setAttribute('skyAngle', '1.309 1.571');
    background.setAttribute('skyColor', '0.0 0.2 0.7 0.0 0.5 1.0 1.0 1.0 1.0');
    scene.appendChild(background);

    navigationInfo = document.createElement('NavigationInfo');
    navigationInfo.setAttribute('id', 'nav');
    navigationInfo.setAttribute('headlight', 'true');
    navigationInfo.setAttribute('type', 'turntable');
    navigationInfo.setAttribute('typeParams', '0 0 0.0 1.56');
//                navigationInfo.setAttribute('visibilityLimit', '0');
    navigationInfo.setAttribute('transitionType', 'TELEPORT');
    scene.appendChild(navigationInfo);

    viewpointGround = document.createElement('Viewpoint');
    viewpointGround.setAttribute('id', 'viewpointGround');
    viewpointGround.setAttribute('orientation', '1 0 0 -1.57');
    viewpointGround.setAttribute('position', '0 ' + elev + ' 0');
    cameraTransform = document.createElement('Transform');
    cameraTransform.setAttribute('rotation', '0 1 0 0');
    cameraTransform.appendChild(viewpointGround);
    scene.appendChild(cameraTransform);

//    inline = document.createElement('inline');
//    inline.setAttribute('nameSpaceName', 'myX3d');
//    inline.setAttribute('url', './ground.x3d');
////    inline.setAttribute('url', './LasVegas.x3d');
//    scene.appendChild(inline);

//    x3dom.debug.doLog('xtile: ' + xtile, x3dom.debug.INFO);
//    x3dom.debug.doLog('ytile: ' + ytile, x3dom.debug.INFO);

//    updateGroundView(lat, lon, rdist);
//    updateGroundView();

    viewpointGround.addEventListener("viewpointChanged", view_ground_changed, false);
    updateGroundView();
}

function updateGroundView() {
    x3dom.debug.doLog('xgrCamPos: ' + grCamPos, x3dom.debug.INFO);
    var x3dElement = document.getElementById('x3dElement');
    var scene = document.getElementById("scene");

    curViewPoint = x3dElement.runtime.viewpoint();
    var rotationCenter = curViewPoint._vf.centerOfRotation;
    var orientation;
    x3dom.debug.doLog('grCamOri: ' + grCamOri, x3dom.debug.INFO);

    var lonDiff = (180 / Math.PI) * rotationCenter.x / EARTH_RADIUS;
    var latDiff = -(180 / Math.PI) * rotationCenter.z / EARTH_RADIUS;
    x3dom.debug.doLog('lonDiff: ' + lonDiff, x3dom.debug.INFO);
    x3dom.debug.doLog('latDiff: ' + latDiff, x3dom.debug.INFO);

    lon += lonDiff;
    lat += latDiff;

    if (grCamOri[0] && grCamOri[1]) {
        orientation = (-grCamOri[0].x)
                + ' ' + (-grCamOri[0].y)
                + ' ' + (-grCamOri[0].z)
                + ' ' + (-grCamOri[1]);
        x3dom.debug.doLog('uuu: ' + grCamOri[0].x, x3dom.debug.INFO);

        // e.g. -0.9999999999999999 0 0 1.5707963267948966
    } else {
        orientation = viewpointGround.getAttribute('orientation');
        // e.g. 1 0 0 -1.57
    }
//    orientation = aOrientation;
    x3dom.debug.doLog('orientation: ' + orientation, x3dom.debug.INFO);

    cameraTransform.removeChild(viewpointGround);
    viewpointGround = document.createElement('Viewpoint');
    viewpointGround.setAttribute('id', 'viewpointGround');
    viewpointGround.setAttribute('orientation', orientation);
//    viewpointGround.setAttribute('orientation', (rightVec + ' -1.57'));
//    viewpointGround.setAttribute('position', 0 + ' ' + processDist(grCamPos) + ' ' + 0);
    viewpointGround.setAttribute('position',
            (grCamPos.x - rotationCenter.x)
            + ' ' + (grCamPos.y - rotationCenter.y)
            + ' ' + (grCamPos.z - rotationCenter.z));
//    viewpointGround.setAttribute('position', 0 + ' ' + grCamPos.y + ' ' + 0);
    viewpointGround.setAttribute('centerOfRotation', '0 0 0');

    cameraTransform.appendChild(viewpointGround);
    viewpointGround.setAttribute('set_bind', 'true');
    viewpointGround.addEventListener("viewpointChanged", view_ground_changed, false);

//    zoom = parseInt(myZConst - Math.log2(grCamPos.y));
    zoom = processZoom(grCamPos);
    var xtile = long2xtile(lon, zoom);
    var ytile = lat2ytile(lat, zoom);
//    x3dom.debug.doLog('xTile: ' + xtile, x3dom.debug.INFO);
//    x3dom.debug.doLog('yTile: ' + ytile, x3dom.debug.INFO);
    var lonTile = xtile2long(xtile, zoom);
    var latTile = ytile2lat(ytile, zoom);
    var lonTilePlus = xtile2long(xtile + 1, zoom);
    var latTilePlus = ytile2lat(ytile + 1, zoom);
    var width = EARTH_RADIUS * (lonTilePlus - lonTile) * Math.PI / 180;
    var height = EARTH_RADIUS * (latTile - latTilePlus) * Math.PI / 180;

    var x = EARTH_RADIUS * (lonTile - lon) * Math.PI / 180;
    var z = EARTH_RADIUS * (lat - latTile) * Math.PI / 180;
//    var x = 0;
//    var z = 0;


////////////////////////////////////////////////////////////////////////////////
//    inline = document.createElement('inline');
//    inline.setAttribute('nameSpaceName', 'myX3d');
//    inline.setAttribute('url', './ground.x3d');
////    inline.setAttribute('url', './LasVegas.x3d');
//    scene.appendChild(inline);
////////////////////////////////////////////////////////////////////////////////

//    navigationInfo = document.createElement('NavigationInfo');
//    navigationInfo.setAttribute('id', 'nav');
//    navigationInfo.setAttribute('headlight', 'true');
//    navigationInfo.setAttribute('type', 'turntable');
//    navigationInfo.setAttribute('typeParams', '0 0 0.0 1.57');
//                navigationInfo.setAttribute('visibilityLimit', '0');
//                navigationInfo.setAttribute('transitionType', 'TELEPORT');


    var size = width + ' ' + height;
//    x3dom.debug.doLog('size: ' + size, x3dom.debug.INFO);
    var translation = (x + width / 2) + ' 0 ' + (z + height / 2);
//    x3dom.debug.doLog('translation: ' + translation, x3dom.debug.INFO);

    var scene = document.getElementById("scene");
    var oldTransform = document.getElementById("x3dTile");

//  <Transform translation="' + translation + '" rotation="1 0 0 -1.5708">
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
    transform = document.createElement('Transform');
    transform.setAttribute('id', "x3dTile");
    transform.setAttribute('translation', translation);
    transform.setAttribute('rotation', "1 0 0 -1.5708");
    transform.appendChild(shape);

    updateCoord(zoom);

    if (oldTransform) {
        scene.removeChild(oldTransform);
    }
    scene.appendChild(transform);
}

function processDist(camCoord) {
    var rdist2 = Math.pow(camCoord.x, 2)
            + Math.pow(camCoord.y, 2)
            + Math.pow(camCoord.z, 2);
    return Math.sqrt(rdist2);
}
function processZoom(camCoord) {
    var rdist2 = Math.pow(camCoord.x, 2)
            + Math.pow(camCoord.y, 2)
            + Math.pow(camCoord.z, 2);
    var rdist = Math.sqrt(rdist2);
    var zoom = parseInt(myZConst - Math.log2(rdist));
    var zoom = Math.min(zoom, 19);
    var zoom = Math.max(zoom, 1);
    return zoom;
}

function view_ground_changed(e) {

    grCamPos = e.position;
//    x3dom.debug.doLog('grCamPos: ' + grCamPos, x3dom.debug.INFO);
    grCamOri = e.orientation;
//    x3dom.debug.doLog('e.orientation: ' + grCamOri[1], x3dom.debug.INFO);
//    rdist2 = Math.pow(grCamPos.x, 2)
//            + Math.pow(grCamPos.y, 2)
//            + Math.pow(grCamPos.z, 2);
//    rdist = Math.sqrt(rdist2);
//    x3dom.debug.doLog('radial distance: ' + rdist, x3dom.debug.INFO);
//                myX3dMode = "earth";
//                x3dom.debug.doLog('Earth must be moved up !!!', x3dom.debug.INFO);
//                document.getElementById('inlineEarth').setAttribute('render', "true");
//
//                viewpointEarth = document.getElementById('viewpointEarth');
//                viewpointEarth.setAttribute('position', '0 0 ' + (rdist - EARTH_RADIUS));
//                viewpointEarth.setAttribute('set_bind', 'true');

    x3dElement = document.getElementById('x3dElement');
    height = x3dElement.runtime.getHeight();
    width = x3dElement.runtime.getWidth();

    zoom = processZoom(grCamPos);
//    x3dom.debug.doLog('zoom: ' + zoom, x3dom.debug.INFO);
    // http://gis.stackexchange.com/questions/12991/how-to-calculate-distance-to-ground-of-all-18-osm-zoom-levels
    // http://wiki.openstreetmap.org/wiki/Zoom_levels
    // The distance represented by one pixel (S) is given by
    // S = C * cos(y) / 2 ^ (z + 8)
    // where...
    // C is the (equatorial) circumference of the Earth
    // z is the zoom level
    // y is the latitude of where you're interested in the scale. 

}

function updateCoord(zoom) {

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

function initEarthMode() {
    navigationInfo = document.createElement('NavigationInfo');
    navigationInfo.setAttribute('id', 'nav');
    navigationInfo.setAttribute('headlight', 'true');
    navigationInfo.setAttribute('type', 'turntable');
    navigationInfo.setAttribute('typeParams', '0 0 1.57 3.14');
    navigationInfo.setAttribute('visibilityLimit', '0');
    navigationInfo.setAttribute('transitionType', 'TELEPORT');
    scene.appendChild(navigationInfo);

    viewpoint = document.createElement('Viewpoint');
    viewpoint.setAttribute('id', 'viewpointEarth');
    viewpoint.setAttribute('position', '0 0 300000');
    scene.appendChild(viewpoint);

    inline = document.createElement('inline');
    inline.setAttribute('nameSpaceName', 'myX3d');
    inline.setAttribute('url', './earth.x3d');
    scene.appendChild(inline);

    viewpoint.addEventListener("viewpointChanged", view_earth_changed, false);
}

function view_earth_changed(e) {
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
    lon = Math.atan(pos.x / pos.z) * 180.0 / Math.PI;
    lat = Math.acos(pos.y / rdist) * 180.0 / Math.PI;
    x3dom.debug.doLog('lon: ' + lon, x3dom.debug.INFO);
    x3dom.debug.doLog('lat: ' + lat, x3dom.debug.INFO);
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

function long2xtile(lon, zoom) {
    return (Math.floor((lon + 180) / 360 * Math.pow(2, zoom)));
}
function lat2ytile(lat, zoom) {
    return (Math.floor((1 - Math.log(Math.tan(lat * Math.PI / 180) + 1 / Math.cos(lat * Math.PI / 180)) / Math.PI) / 2 * Math.pow(2, zoom)));
}
function xtile2long(x, z) {
    return (x / Math.pow(2, z) * 360 - 180);
}
function ytile2lat(y, z) {
    var n = Math.PI - 2 * Math.PI * y / Math.pow(2, z);
    return (180 / Math.PI * Math.atan(0.5 * (Math.exp(n) - Math.exp(-n))));
}
