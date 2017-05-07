<?php

/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
$fondo1 = $_POST['fondo'];
if(empty($fondo1)){ die("manca fondo scala misuratore 1!");}
$fondo2 = $_POST['fondo1'];
if(empty($fondo2)){ die("manca fondo scala misuratore 2!");}
$unita = $_POST['unita'];
if(empty($unita)){ die("manca unità di misura!");}
$freq = $_POST['freq'];
if(empty($freq)){ die("manca frequenza di misura!");}
$upd = $_POST['upd'];
if(empty($upd)){ die("manca frequenza di caricamento!");}
$inizio = $_POST['inizio'];
if(empty($inizio)){ $inizio="NA";}
$fine = $_POST['fine'];
if(empty($fine)){ $fine="NA";}
if($inizio > $fine){ die("data di fine precedente a data di inizio!");}
$conf_file = fopen("config.txt", "w");
$post = $_POST['post'];
$dmis = $_POST['dmis'];
$dup = $_POST['dup'];

$txt ="inizio\n".$fondo1."\n".$fondo2."\n".$unita."\n".$freq."\n".$upd."\n".$inizio."\n".$fine."\n".$post."\n".$dmis."\n".$dup."\nfine";
echo $txt;
fwrite($conf_file,$txt);
fclose($conf_file);
echo '<br>caricamento riuscito';
header("Location: google_graph.html");
?>