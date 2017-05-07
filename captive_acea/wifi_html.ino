//FILE  per Pagina Captive Portal WI-FI
String responseHTML = ""
  "<!DOCTYPE html><html><head><meta http-equiv=\"refresh\" content=\"10\"><title>Portale Arduino</title></head><body>"
  "<h1>Periferica Step-Test:</h1><p><b>Segnale GPRS: </b><br>";
//DISEGNO svg tacche segnale
String CSQs(){
  String str = "";
  int csq=csqRead();
  if(csq<0){
    str+="<svg width=\"30\"height=\"50\"><rect y=\"48\"width=\"20\"height=\"2\"style=\"fill:rgb(255,0,0)\"/></svg>";
    str+="<svg width=\"30\"height=\"50\"><rect y=\"48\"width=\"20\"height=\"2\"style=\"fill:rgb(255,0,0)\"/></svg>";
    str+="<svg width=\"30\"height=\"50\"><rect y=\"48\"width=\"20\"height=\"2\"style=\"fill:rgb(255,0,0)\"/></svg>";
    str+="<svg width=\"30\"height=\"50\"><rect y=\"48\"width=\"20\"height=\"2\"style=\"fill:rgb(255,0,0)\"/></svg>";
  }
  if(csq>0 &&csq<=7){
    str+="<svg width=\"30\"height=\"50\"><rect y=\"40\"width=\"20\"height=\"10\"style=\"fill:rgb(0,0,255)\"/></svg>";
    str+="<svg width=\"30\"height=\"50\"><rect y=\"48\"width=\"20\"height=\"2\"style=\"fill:rgb(0,0,255)\"/></svg>";
    str+="<svg width=\"30\"height=\"50\"><rect y=\"48\"width=\"20\"height=\"2\"style=\"fill:rgb(0,0,255)\"/></svg>";
    str+="<svg width=\"30\"height=\"50\"><rect y=\"48\"width=\"20\"height=\"2\"style=\"fill:rgb(0,0,255)\"/></svg>";  }
  if(csq>7&&csq<=14){
    str+="<svg width=\"30\"height=\"50\"><rect y=\"40\"width=\"20\"height=\"10\"style=\"fill:rgb(0,0,255)\"/></svg>";
    str+="<svg width=\"30\"height=\"50\"><rect y=\"27\"width=\"20\"height=\"23\"style=\"fill:rgb(0,0,255)\"/></svg>";
    str+="<svg width=\"30\"height=\"50\"><rect y=\"48\"width=\"20\"height=\"2\"style=\"fill:rgb(0,0,255)\"/></svg>";
    str+="<svg width=\"30\"height=\"50\"><rect y=\"48\"width=\"20\"height=\"2\"style=\"fill:rgb(0,0,255)\"/></svg>";
    
  }
  if(csq>14&&csq<=21){
    str+="<svg width=\"30\"height=\"50\"><rect y=\"40\"width=\"20\"height=\"10\"style=\"fill:rgb(0,0,255)\"/></svg>";
    str+="<svg width=\"30\"height=\"50\"><rect y=\"27\"width=\"20\"height=\"23\"style=\"fill:rgb(0,0,255)\"/></svg>";
    str+="<svg width=\"30\"height=\"50\"><rect y=\"15\"width=\"20\"height=\"35\"style=\"fill:rgb(0,0,255)\"/></svg>";
    str+="<svg width=\"30\"height=\"50\"><rect y=\"48\"width=\"20\"height=\"2\"style=\"fill:rgb(0,0,255)\"/></svg>";
  }
  if(csq>21){
    str+="<svg width=\"30\"height=\"50\"><rect y=\"40\"width=\"20\"height=\"10\"style=\"fill:rgb(0,0,255)\"/></svg>";
    str+="<svg width=\"30\"height=\"50\"><rect y=\"27\"width=\"20\"height=\"23\"style=\"fill:rgb(0,0,255)\"/></svg>";
    str+="<svg width=\"30\"height=\"50\"><rect y=\"15\"width=\"20\"height=\"35\"style=\"fill:rgb(0,0,255)\"/></svg>";
    str+="<svg width=\"30\"height=\"50\"><rect width=\"20\"height=\"50\"style=\"fill:rgb(0,0,255)\"/></svg>";
  }
  return str;
}
//FUNZIONE  construzione della pagina HTML
String HtmlPage(){
  String str = responseHTML+String(csqRead())+"</p>";
  str+=CSQs();
  str+= "<br><br>Stato Corrente: ";
  if(now()>=DataI &&now()<=DataO){
    str += "Modalità Step-Test";
    }else{
      if(conf[2]){ str+= "Modalità data-logger";}else{str+="Modalità bassa energia/trasmittente spenta";}
    }
  str+= "<br><br>Ultimo caricamento: "+lastTime+"<br><br>";
  str+="Configurazione periferica:<br>Intervallo misura: "+String(conf[0])+" s<br>Intervallo Caricamento: "+String(conf[1])+" s<br>";
  str+="Data/Ora Inizio: "+timeFormat(DataI)+"<br>Data/Ora Fine: "+timeFormat(DataO)+"<br>Dopo step-test: <br><hr>";
  if(conf[2]){
  str+="<b>Data Logger</b><br>";
  str+="Intervallo misura: "+String(conf[3])+" s<br>Intervallo Caricamento: "+String(conf[4])+" s<br>";
  }else{str+="<b>Trasmittente Spenta</b><br>";}
  str+="<hr>Lettura Corrente CH1: "+ String(pcfRead(P0)/25.5)+" V, "+String(pcfRead(P0)/255.0*fondo1)+" "+unit+"<br>";
  str+="Lettura Corrente CH2: "+ String(pcfRead(P1)/25.5)+" V, "+String(pcfRead(P1)/255.0*fondo2)+" "+unit+"<hr>";
  str+="<center><a href=\"wifioff\"><button>Disattiva Wi-Fi</button></a></center>";
  str+="<br><br> debug: <i>"+ stato+"</i>";
  str += "</body></html>";
  return str;
}
