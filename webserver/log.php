<?php
$mis = $_GET['misura1'];
$mis1 = $_GET['misura2'];
$ora = date(DATE_ATOM);
if(file_exists("log.csv")){
$csv = fopen("log.csv","a+");
fwrite($csv, $ora.",".$mis.",".$mis1."\n");
fclose($csv);
}else{
$csv = fopen("log.csv","w");
fwrite($csv, "Data,Portata1,Portata2\n");
fwrite($csv, $ora.",".$mis.",".$mis1."\n");
fclose($csv);
}
echo "OK";
?>