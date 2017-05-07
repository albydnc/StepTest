<?php
require 'PHPMailerAutoload.php';
$addr = $_POST['addr'];
if(!empty($addr)){
$mail = new PHPMailer;
$mail->From = "steptest.donotreply@acea.com";
$mail->FromName = "Periferica Step-Test";
$mail->addAddress($addr);
$mail->isHTML(true);
$mail->Subject = "Archivio CSV Step-Test";
$mail->Body = "<i>Archivio CSV della periferica Step-Test</i>";
$mail->addAttachment("log.csv");
if(!$mail->send()) 
{
    echo "errore: " . $mail->ErrorInfo;
} 
else 
{
    echo "mail inviata<br>";
    unlink('log.csv');
}
}else{
unlink('log.csv');
}
echo "archivio eliminato";
?>