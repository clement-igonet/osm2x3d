<!DOCTYPE html>
<html>
    <head>
        <title>OSM to X3D</title>
        <script src="http://www.x3dom.org/download/dev/x3dom-full.js"></script>
        <script src="jquery-2.1.3.min.js"></script>
        <script src="myJS.js"></script>
        <link rel="stylesheet" href="http://www.x3dom.org/download/dev/x3dom.css">
        <link rel='stylesheet' type='text/css' href='style.css'></link>
    </head>
    <body id="body">
        <?php
        if ($_GET['type'] == "bbox") {
            $url = "osm2x3d.php?"
                    . "ll_lon=" . $_GET['ll_lon'] . "&"
                    . "ll_lat=" . $_GET['ll_lat'] . "&"
                    . "ur_lon=" . $_GET['ur_lon'] . "&"
                    . "ur_lat=" . $_GET['ur_lat'];
        } else if ($_GET['type'] == "osm") {
            // addr=18/48.86406/2.28953
            // input:$lat,$lon,$zoom
            $info = explode("/", $_GET['addr']);
            $zoom = $info[0];
            $lat = $info[1];
            $lon = $info[2];

            $width = 800;
            $height = 300;
            $tile_size = 256;
            $xtile = floor((($lon + 180) / 360) * pow(2, $zoom));
            $ytile = floor((1 - log(tan(deg2rad($lat)) + 1 / cos(deg2rad($lat))) / pi()) / 2 * pow(2, $zoom));
            $xtile_s = ($xtile * $tile_size - $width / 2) / $tile_size;
            $ytile_s = ($ytile * $tile_size - $height / 2) / $tile_size;
            $xtile_e = ($xtile * $tile_size + $width / 2) / $tile_size;
            $ytile_e = ($ytile * $tile_size + $height / 2) / $tile_size;
            $n = pow(2, $zoom);
            $lon_s = $xtile_s / $n * 360.0 - 180.0;
            $lat_s = rad2deg(atan(sinh(pi() * (1 - 2 * $ytile_s / $n))));
            $lon_e = $xtile_e / $n * 360.0 - 180.0;
            $lat_e = rad2deg(atan(sinh(pi() * (1 - 2 * $ytile_e / $n))));
            $url = "osm2x3d.php?"
                    . "ll_lon=" . $lon_s . "&"
                    . "ll_lat=" . $lat_e . "&"
                    . "ur_lon=" . $lon_e . "&"
                    . "ur_lat=" . $lat_s;
        } else if ($_GET['type'] == "gmap") {
            // addr=44.9596242,-93.1577772,13z
            $info = explode(",", $_GET['addr']);
            $lat = $info[0];
            $lon = $info[1];
            $zoom = substr($info[2], 0, strlen($info[2] - 1));

            $width = 1600;
            $height = 600;
            $tile_size = 256;
            $xtile = floor((($lon + 180) / 360) * pow(2, $zoom));
            $ytile = floor((1 - log(tan(deg2rad($lat)) + 1 / cos(deg2rad($lat))) / pi()) / 2 * pow(2, $zoom));
            $xtile_s = ($xtile * $tile_size - $width / 2) / $tile_size;
            $ytile_s = ($ytile * $tile_size - $height / 2) / $tile_size;
            $xtile_e = ($xtile * $tile_size + $width / 2) / $tile_size;
            $ytile_e = ($ytile * $tile_size + $height / 2) / $tile_size;
            $n = pow(2, $zoom);
            $lon_s = $xtile_s / $n * 360.0 - 180.0;
            $lat_s = rad2deg(atan(sinh(pi() * (1 - 2 * $ytile_s / $n))));
            $lon_e = $xtile_e / $n * 360.0 - 180.0;
            $lat_e = rad2deg(atan(sinh(pi() * (1 - 2 * $ytile_e / $n))));
            $url = "osm2x3d.php?"
                    . "ll_lon=" . $lon_s . "&"
                    . "ll_lat=" . $lat_e . "&"
                    . "ur_lon=" . $lon_e . "&"
                    . "ur_lat=" . $lat_s;
        }
        ?>
        <div id="header">
            osm2x3d
        </div>
        <div id="container">
            <X3D id="the_element" showlog="false" showstat="false">
                <Scene id="scene">
                    <inline id="inline"></inline>
                </Scene>             
            </X3D>
        </div>
        <div id="content"> info </div>
        <script>
            document.onload = function () {
                document.getElementById('inline').setAttribute(
                        'url',
<?php echo '                        "' . $url . '"' . "\r\n"; ?>
                );
            }
        </script>
    </body>
</html>

