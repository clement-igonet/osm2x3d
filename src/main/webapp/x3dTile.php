<?php

header('Content-Type: text/xml');

// TODO: convert zomm/xtile/ytile to lat/lon
$zoom = $_GET['zoom'];
$xtile = $_GET['xtile'];
$ytile = $_GET['ytile'];

$n = pow(2, $zoom);

$ll_lon = $xtile / $n * 360.0 - 180.0;
$ur_lon = ($xtile + 1) / $n * 360.0 - 180.0;

$ur_lat = rad2deg(atan(sinh(pi() * (1 - 2 * $ytile / $n))));
$ll_lat = rad2deg(atan(sinh(pi() * (1 - 2 * ($ytile + 1) / $n))));

$url = 'http://api.openstreetmap.org/api/0.6/map?bbox=';
$url .= $ll_lon . "," . $ll_lat . "," . $ur_lon . "," . $ur_lat;

$stdin = file_get_contents($url);

error_log("osm url:".$url, 0);

$command = dirname(__FILE__) . "/bin/converter";







// $command is the command to run, $stdin is your input.  You get back $stdout and $stderr and $returnValue
// you'll probably want to wrap this in a function ;)

$descriptorSpec = array(0 => array("pipe", "r"),
                           1 => array('pipe', 'w'),
                           2 => array('pipe', 'w'));
  $process = proc_open($command, $descriptorSpec, $pipes);
  $txOff = 0; $txLen = strlen($stdin);
  $stdout = ''; $stdoutDone = FALSE;
  $stderr = ''; $stderrDone = FALSE;
  stream_set_blocking($pipes[0], 0); // Make stdin/stdout/stderr non-blocking
  stream_set_blocking($pipes[1], 0);
  stream_set_blocking($pipes[2], 0);
  if ($txLen == 0) fclose($pipes[0]);
  while (TRUE) {
    $rx = array(); // The program's stdout/stderr
    if (!$stdoutDone) $rx[] = $pipes[1];
    if (!$stderrDone) $rx[] = $pipes[2];
    $tx = array(); // The program's stdin
    if ($txOff < $txLen) $tx[] = $pipes[0];
    $ex = NULL;
    stream_select($rx, $tx, $ex, NULL, NULL); // Block til r/w possible
    if (!empty($tx)) {
      $txRet = fwrite($pipes[0], substr($stdin, $txOff, 8192));
      if ($txRet !== FALSE) $txOff += $txRet;
      if ($txOff >= $txLen) fclose($pipes[0]);
    }
    foreach ($rx as $r) {
      if ($r == $pipes[1]) {
        $out = fread($pipes[1], 8192);

//echo <<<EOF
//$out
//EOF;

//        error_log("DEBUG - stdout: ".$out, 0);
        $stdout .= $out;
        if (feof($pipes[1])) { fclose($pipes[1]); $stdoutDone = TRUE; }
      } else if ($r == $pipes[2]) {
        $stderr .= fread($pipes[2], 8192);
        if (feof($pipes[2])) { fclose($pipes[2]); $stderrDone = TRUE; }
      }
    }
    if (!is_resource($process)) break;
    if ($txOff >= $txLen && $stdoutDone && $stderrDone) break;
  }
  $returnValue = proc_close($process);

error_log("DEBUG - returnValue: ".$returnValue, 0);

if (!empty($stderr)) {
    error_log("ERROR - ".$stderr, 0);
}

/* $stdout = <<<XML
<?xml version='1.0'?> 
<document>
 <title>Forty What?</title>
 <from>Joe</from>
 <to>Jane</to>
 <body>
  I know that's the answer -- but what's the question?
 </body>
</document>
XML;
*/

echo $stdout;

//$sxe = new SimpleXMLElement($stdout);
//echo $sxe->asXML();



?>
