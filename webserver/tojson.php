<?php
$ndat = $_GET['n'];
$fondo = $_GET['f1'];
$fondo1 = $_GET["f2"];
$csv = array_map('str_getcsv', file('log.csv',FILE_IGNORE_NEW_LINES));
$arr = array("Data", "Portata1","Portata2");

array_shift($csv);
if(count($csv)-$ndat-1 > 0){
for($i = 0; $i < count($csv)-$ndat;++$i){
array_shift($csv);
}
}
array_unshift($csv,$arr);
for($i = 1; $i < count($csv);++$i){
$csv[$i][1]=round(($csv[$i][1]*$fondo/100.0),2);
if(empty($fondo1)){$csv[$i][2]=0.0;}else{$csv[$i][2]=round(($csv[$i][2]*$fondo1/100.0),2);}
}
echo json_encode($csv,JSON_NUMERIC_CHECK|JSON_PRESERVE_ZERO_FRACTION);
/*echo"[[new ".$csv[1][0].",".$csv[1][1]."]";
for($i = 2, $size = count($csv); $i < $size; ++$i) {
    echo ",[new ".$csv[$i][0].",".$csv[$i][1]."]";
}
echo "]";*/
?>