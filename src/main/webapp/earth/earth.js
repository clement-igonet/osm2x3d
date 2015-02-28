
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
myZConst = 19;
var lon = 0;
var lat = Math.PI / 2;

document.onload = function () {
    switch (myX3dMode) {
        case EARTH_MODE:
            initEarthMode();
            break;
        case GROUND_MODE:
            initGroundMode(41.632653061224495, 3.214285714285846, 79428.6318524534);
            break;
        default:
    }

    document.addEventListener('keydown', function (event) {
        // 'x' keydown
        if (event.keyCode == 88) {
            updateGroundView(41.632653061224495, 3.214285714285846, 79428.6318524534);
//                        element = document.getElementById('x3dElement');
//                        bindable = element.runtime.getActiveBindable('viewpoint');
//                        x3dom.debug.doLog('viewpoint.position: ' + bindable.getAttribute('position'));
        }
    }, false);
}

function initGroundMode(lat, lon, rdist) {

    background = document.createElement('Background');
    background.setAttribute('groundColor', '0.972 0.835 0.666');
    background.setAttribute('skyAngle', '1.309 1.571');
    background.setAttribute('skyColor', '0.0 0.2 0.7 0.0 0.5 1.0 1.0 1.0 1.0');
    scene.appendChild(background);

    navigationInfo = document.createElement('NavigationInfo');
    navigationInfo.setAttribute('id', 'nav');
    navigationInfo.setAttribute('headlight', 'true');
    navigationInfo.setAttribute('type', 'turntable');
    navigationInfo.setAttribute('typeParams', '0 0 0.0 1.57');
//                navigationInfo.setAttribute('visibilityLimit', '0');
//                navigationInfo.setAttribute('transitionType', 'TELEPORT');
    scene.appendChild(navigationInfo);

    viewpoint = document.createElement('Viewpoint');
    viewpoint.setAttribute('id', 'viewpointGround');
    viewpoint.setAttribute('orientation', '1 0 0 -1.57');
    viewpoint.setAttribute('position', '0 ' + rdist + ' 0');
    transform = document.createElement('Transform');
    transform.setAttribute('rotation', '0 1 0 0');
    transform.appendChild(viewpoint);
    scene.appendChild(transform);

//    inline = document.createElement('inline');
//    inline.setAttribute('nameSpaceName', 'myX3d');
//    inline.setAttribute('url', './ground.x3d');
////    inline.setAttribute('url', './LasVegas.x3d');
//    scene.appendChild(inline);

//    x3dom.debug.doLog('xtile: ' + xtile, x3dom.debug.INFO);
//    x3dom.debug.doLog('ytile: ' + ytile, x3dom.debug.INFO);

//    updateGroundView(lat, lon, rdist);

    viewpoint.addEventListener("viewpointChanged", view_ground_changed, false);
}

function updateGroundView(lat, lon, rdist) {
    var zoom = parseInt(myZConst - Math.log2(rdist));
    var xtile = long2xtile(lon, zoom);
    var ytile = lat2ytile(lat, zoom);
    var url = 'http://a.tile.openstreetmap.org/' + zoom + '/' + xtile + '/' + ytile + '.png';
    var lonTile = xtile2long(xtile, zoom);
    var latTile = ytile2lat(ytile, zoom);
    var width = xtile2long(xtile + 1, zoom) - lonTile;
    var height = ytile2lat(ytile + 1, zoom) - latTile;
    var z = EARTH_RADIUS * (lat - latTile) * Math.PI / 180;
    var x = EARTH_RADIUS * (lonTile - lon) * Math.PI / 180;
//                        <Rectangle2D size='60010.812 40093.445'></Rectangle2D>
    var size = width + ' ' + height;
    var translation = x + ' 0 ' + z;
    x3dom.debug.doLog('url: ' + url, x3dom.debug.INFO);
    x3dom.debug.doLog('size: ' + size, x3dom.debug.INFO);
    x3dom.debug.doLog('translation: ' + translation, x3dom.debug.INFO);

//    inline = document.createElement('inline');
//    inline.setAttribute('nameSpaceName', 'myX3d');
//    inline.setAttribute('url', './ground.x3d');
////    inline.setAttribute('url', './LasVegas.x3d');
//    scene.appendChild(inline);

//                <x3d id="x3dElement" showlog="true" showstat="false">
//                <Scene id="scene">
//                </Scene>             
//            </x3d>
//    var myDiv = "<x3d id='x3dElement' showlog='true' showstat='false'>";
    var myDiv = '\
            <x3d>\
                <Scene id="scene">\
                    <Transform translation="0 0 0" rotation="1 0 0 -1.5708">\
                        <Shape>\
                            <Appearance>\
                                <Material diffuseColor="1 0 0"></Material>\
                            </Appearance>\
                            <Rectangle2D size="60010.812 40093.445"></Rectangle2D>\
                        </Shape>\
                    </Transform>\
                </Scene>\
            </x3d>';
    x3dom.debug.doLog(myDiv, x3dom.debug.INFO);
    console.log(myDiv);
//    var x3d = document.getElementsByTagName("x3dElement")[0];
    var x3d = document.getElementById("x3dElement");
    x3d.parentNode.removeChild(x3d);
    var div = document.getElementById("x3dContainer");
    div.innerHTML = myDiv;
    x3dom.reload();
//    
//        <Transform translation='0 0 0' rotation='1 0 0 -1.5708'>
//            <Group>
//                <Shape>
//                    <Appearance>
//                        <ImageTexture url='"http://a.tile.openstreetmap.org/16/11801/25709.png"'/>
//                    </Appearance>
//                    <Rectangle2D size='60010.812 40093.445'></Rectangle2D>
//                </Shape>
//            </Group>
//        </Transform>


//    var x3dTile = "\
//                    <Transform translation='" + translation + "' rotation='1 0 0 -1.5708'>\
//                        <Group>\
//                            <Shape>\
//                                <Appearance>\
//                                    <ImageTexture url='\"" + url + "\"'/>\
//                                </Appearance>\
//                                <Rectangle2D size='" + size + "'></Rectangle2D>\
//                            </Shape>\
//                        </Group>\
//                    </Transform>";
//    scene.innerHTML = x3dTile;
//    x3dom.reload();
//            scene.appendChild(x3dTileTextNode);

//                double z = RADIUS * (OsmWorld::getInstance()->origin_.first - nodeIt->lat_) * (M_PI / 180);
//                double x = RADIUS * (nodeIt->lon_ - OsmWorld::getInstance()->origin_.second) * (M_PI / 180);

}

function view_ground_changed(e) {

    var pos = e.position;
    x3dom.debug.doLog('viewpoint position: ' + pos, x3dom.debug.INFO);
    rdist2 = Math.pow(pos.x, 2) + Math.pow(pos.y, 2) + Math.pow(pos.z, 2);
    rdist = Math.sqrt(rdist2);
    x3dom.debug.doLog('radial distance: ' + rdist, x3dom.debug.INFO);
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

    zoom = parseInt(myZConst - Math.log2(rdist));
    zoom = Math.min(zoom, 19);
    zoom = Math.max(zoom, 1);

//                        x3dom.debug.doLog('height: ' + height, x3dom.debug.INFO);
//                        x3dom.debug.doLog('width: ' + width, x3dom.debug.INFO);
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

function initEarthMode() {
    navigationInfo = document.createElement('NavigationInfo');
    navigationInfo.setAttribute('id', 'nav');
    navigationInfo.setAttribute('headlight', 'true');
    navigationInfo.setAttribute('type', 'turntable');
    navigationInfo.setAttribute('typeParams', '0 0 0 3.14');
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
