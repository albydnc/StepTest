//funzione Reset
void modemReset(){
  #ifdef DEBUG
  Serial.println("Resetto il modem");
  #endif
  blinkled(5);
  gprs.println("AT+CFUN=0\r");
  if(!gprs.find("OK"));
  delay(2000);
  gprs.println("AT+CFUN=1\r");
  if(!gprs.find("OK"));
  cellConnect();
}
void modemOFF(){
  gprs.println("AT+CFUN=0\r");
  if(!gprs.find("OK"));
}
void modemON(){
  gprs.println("AT+CFUN=1\r");
  if(!gprs.find("OK"));
  cellConnect();
}
//                                                       funzione connessione alla rete GSM/GPRS
void cellConnect(){
  #ifdef DEBUG
  Serial.print("In connessione");
  #endif
  do{
  delay(5000);
  gprs.println("AT+CREG?\r");
  #ifdef DEBUG
  Serial.print(".");
  #endif
  }while(!gprs.find("0,1"));
  #ifdef DEBUG
  Serial.println("Connesso al ponte radio");
  #endif
  delay(1000);
  #ifdef DEBUG
  Serial.println("Configuro GPRS");
  #endif
  gprs.println("AT+SAPBR=0,1\r");
  if(!gprs.find("OK")){
  #ifdef DEBUG
  Serial.println("Errore riavvio GPRS");
  #endif
  }
  delay(600);
  gprs.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"\r");
  while(!gprs.find("OK")){
  #ifdef DEBUG
  Serial.print(".");
  #endif
  }
  delay(200);
  #ifdef TIM
  gprs.println("AT+SAPBR=3,1,\"APN\",\"wap.tim.it\"\r");
  #endif
  #ifdef VODAFONE
  gprs.println("AT+SAPBR=3,1,\"APN\",\"web.omnitel.it\"\r");
  #endif
  while(!gprs.find("OK")){
  #ifdef DEBUG
  Serial.print(",");
  #endif
  }
  delay(200);
  gprs.println("AT+SAPBR=1,1\r");
  if(!gprs.find("OK")){
  #ifdef DEBUG
  Serial.println("Errore apertura GPRS");
  Serial.println("Configurazione Completata, GPRS Attivo");
  #endif
  }
  stato = "GPRS COMPLETATO";
  delay(200);
}
//FUNZIONE lettura intensità segnale
int csqRead(){
  gprs.println("AT+CSQ\r");
  gprs.readStringUntil(':');
  int csq = gprs.readStringUntil(',').toInt();
  int err = gprs.readStringUntil('\r').toInt();
  if(err >0) csq=-1;
  return csq;
}
//                                                FUNZIONE invio dati tramite HTTP GET all'url indicata
void dataSend(float mis, float mis1,String url,bool ssl){
  #ifdef DEBUG
  Serial.println("Invio dati");
  #endif
  url+=String(mis);
  url+="&misura2=";
  url+=String(mis1);
  gprs.println("AT+HTTPINIT\r");
  if(!gprs.find("OK")){
  modemReset();
  return;
  }
  delay(200);
  if(ssl){
  gprs.println("AT+HTTPSSL=1\r");
  if(!gprs.find("OK")){
  modemReset();
  return;
  }
  }
  delay(200);
  gprs.println("AT+HTTPPARA=\"CID\",1\r");
  delay(200);
  if(!gprs.find("OK")){
  modemReset();
  return;
  }
  delay(200);
  gprs.println("AT+HTTPPARA=\"URL\",\""+url+"\"\r");
  if(!gprs.find("OK")){
  modemReset();
  return;
  }
  delay(500);
  gprs.println("AT+HTTPACTION=0\r");
  if(!gprs.find("OK")){
  modemReset();
  return;
  }
  delay(1000);
  lastTime = timeFormat(now());
  if(!gprs.find(":0,200")){ 
    #ifdef DEBUG
    Serial.println("Trasmissione fallita!!!");
    #endif
    lastTime += "  <b>NON RIUSCITO</b>";
  }else{
    #ifdef DEBUG
    Serial.println("HTTP 200 OK");
    #endif
    lastTime += "  <b>RIUSCITO</b>";
  }
  gprs.println("AT+HTTPREAD\r");
  if(!gprs.find("HTTPREAD")){
  modemReset();
  digitalWrite(2, HIGH);
  return;
  } 
  int tconf[6];
  gprs.readStringUntil(';');
  tconf[0]  = gprs.readStringUntil('-').toInt();           //legge data ultima config
  tconf[1]  = gprs.readStringUntil('-').toInt();
  tconf[2]  = gprs.readStringUntil('T').toInt();
  tconf[3]  = gprs.readStringUntil(':').toInt();
  tconf[4]  = gprs.readStringUntil(':').toInt();
  tconf[5]  = gprs.readStringUntil('+').toInt();
  confver = tmConvert_t(tconf[0],tconf[1],tconf[2],tconf[3],tconf[4],tconf[5]);
  gprs.println("AT+HTTPTERM\r");
  if(!gprs.find("OK")){
  modemReset();
  return;
  }
  #ifdef DEBUG
  Serial.println("HTTP concluso");
  #endif
  stato = "invio COMPLETATO";
}

//                                                       FUNZIONE download file di configurazione 
bool confGet(String url,bool ssl){
  digitalWrite(2, LOW);
  #ifdef DEBUG
  Serial.println("Scarico Configurazione");
  #endif
  bool c = false;
  gprs.println("AT+HTTPINIT\r");
  if(!gprs.find("OK")){
  modemReset();
  return c;
  }
  delay(200);
  if(ssl){
  gprs.println("AT+HTTPSSL=1\r");
  if(!gprs.find("OK")){
  modemReset();
  return c;
  }
  }
  delay(200);
  gprs.println("AT+HTTPPARA=\"CID\",1\r");
  if(!gprs.find("OK")){
  modemReset();
  return c;
  }
  delay(200);
  gprs.println("AT+HTTPPARA=\"URL\",\""+url+"\"\r");
  if(!gprs.find("OK")){
  modemReset();
  return c;
  }
  delay(500);
  gprs.println("AT+HTTPACTION=0\r");
  if(!gprs.find("OK")){
  modemReset();
  return c;
  }
  delay(2000);
  if(!gprs.find(":0,200")){ 
    #ifdef DEBUG
    blinkled(10);
    Serial.println("Ricezione fallita!!!");
    #endif
    c=false;
    goto term;
  }else{
    #ifdef DEBUG
    Serial.println("HTTP 200 OK");
    #endif
  }
  gprs.println("AT+HTTPREAD\r");
  if(!gprs.find("HTTPREAD")){
  modemReset();
  digitalWrite(2, HIGH);
  return c;
  } 
  gprs.readStringUntil('i');
  gprs.readStringUntil('\n');
  fondo1 = gprs.readStringUntil('\n').toInt();//legge fondo scala misuratore 1
  fondo2 = gprs.readStringUntil('\n').toInt();//legge fondo scala misuratore 2
  unit = gprs.readStringUntil('\n'); //legge unità di misura
conf[0] = gprs.readStringUntil('\n').toInt();//legge tempo misura
conf[1] = gprs.readStringUntil('\n').toInt();//legge tempo upload
int tinizio[5];
int tfine[5];
  tinizio[0]  = gprs.readStringUntil('-').toInt();//legge data inizio
  tinizio[1]  = gprs.readStringUntil('-').toInt();
  tinizio[2]  = gprs.readStringUntil('T').toInt();
  tinizio[3]  = gprs.readStringUntil(':').toInt();
  tinizio[4]  = gprs.readStringUntil('\n').toInt();
  DataI = tmConvert_t(tinizio[0],tinizio[1],tinizio[2],tinizio[3],tinizio[4],0);
  tfine[0]  = gprs.readStringUntil('-').toInt();//legge data fine
  tfine[1]  = gprs.readStringUntil('-').toInt();
  tfine[2]  = gprs.readStringUntil('T').toInt();
  tfine[3]  = gprs.readStringUntil(':').toInt();
  tfine[4]  = gprs.readStringUntil('\n').toInt();
  DataO = tmConvert_t(tfine[0],tfine[1],tfine[2],tfine[3],tfine[4],0);
conf[2] = gprs.readStringUntil('\n').toInt();//legge cosa fare dopo
conf[3] = gprs.readStringUntil('\n').toInt();//legge tempo misura
conf[4] = gprs.readStringUntil('\n').toInt();//legge tempo upload
if(!gprs.find("OK")) goto term;
delay(500);
  c=true;
  term:
  gprs.println("AT+HTTPTERM\r");
  if(!gprs.find("OK")){
  modemReset();
  return c;
  }
  #ifdef DEBUG
  Serial.println("HTTP concluso");
  #endif
  stato = "configurazione COMPLETATA";
  if(c) lastconf=now();
  return c;
}

                                                                   //FUNZIONE sincronizzazione orologio via Internet
time_t setDateTimeWeb(){
  blinkled(1);
  gprs.println("AT+HTTPINIT\r");
  delay(200);
  if(!gprs.find("OK")){
  modemReset();
  return 0;
  }
  delay(200);
  blinkled(1);
  gprs.println("AT+HTTPPARA=\"CID\",1\r");
  delay(200);
  if(!gprs.find("OK")){
  modemReset();
  return 0;
  }
  delay(200);
  blinkled(1);
  gprs.println("AT+HTTPPARA=\"URL\",\"http://precursori.altervista.org/php_test/localtime.php\"\r");
  if(!gprs.find("OK")){
  modemReset();
  return 0;
  }
  delay(500);
  blinkled(1);
  gprs.println("AT+HTTPACTION=0\r");
  if(!gprs.find("OK")){
  modemReset();
  return 0;
  }
  delay(1000);
  if(!gprs.find(":0,200")){ 
  gprs.println("AT+HTTPTERM\r");
  if(!gprs.find("OK")){
  modemReset();
  return 0;
  }
  #ifdef DEBUG
  Serial.println("HTTP concluso");
  #endif
    return 0;
  }else{
    #ifdef DEBUG
    Serial.println("HTTP 200 OK");
    #endif
  }
  delay(200);
  gprs.println("AT+HTTPREAD\r");
  if(!gprs.find("HTTPREAD")){
  modemReset();
  return 0;
  }
  delay(500);
  int t[5];
  gprs.readStringUntil('/');
  t[0]  = gprs.readStringUntil('-').toInt();//legge data inizio
  t[1]  = gprs.readStringUntil('-').toInt();
  t[2]  = gprs.readStringUntil(' ').toInt();
  t[3]  = gprs.readStringUntil(':').toInt();
  t[4]  = gprs.readStringUntil(':').toInt();
  t[5]  = gprs.readStringUntil('\n').toInt();
  time_t dataora = tmConvert_t(t[0],t[1],t[2],t[3],t[4],t[5]);
  blinkled(1);
while(!gprs.find("OK"));
delay(200);
  gprs.println("AT+HTTPTERM\r");
  if(!gprs.find("OK")){
  modemReset();
  return 0;
  }
  #ifdef DEBUG
  Serial.println("HTTP concluso");
  #endif
  stato = "Orologio COMPLETATO";
  blinkled(2);
  return dataora;
}

time_t setDateTimeGPRS(){
  gprs.println("AT+CCLK?\r");
  gprs.readStringUntil('"');
  byte yr = gprs.readStringUntil('/').toInt();
  byte M = gprs.readStringUntil('/').toInt();
  byte dy = gprs.readStringUntil(',').toInt();
  byte hr = gprs.readStringUntil(':').toInt();
  byte mn = gprs.readStringUntil(':').toInt();
  byte se = gprs.readStringUntil('+').toInt();
  return tmConvert_t(2000+yr,M,dy,hr,mn,se);
}
//FUNZIONE conversione data in Unix-Epoch
time_t tmConvert_t(int YYYY, byte MM, byte DD, byte hh, byte mm, byte ss)
{
  tmElements_t tmSet;
  tmSet.Year = YYYY - 1970;
  tmSet.Month = MM;
  tmSet.Day = DD;
  tmSet.Hour = hh;
  tmSet.Minute = mm;
  tmSet.Second = ss;
  return makeTime(tmSet); 
}
//FUNZIONE formattazione data/ora human-readable
String timeFormat(time_t tm){
  String tim = String(hour(tm))+":";
  if(minute(tm)<10) tim+="0";
  tim+=String(minute(tm))+":";
  if(second(tm)<10) tim+="0";
  tim+=String(second(tm))+" ";
  tim+=String(day(tm))+"/"+String(month(tm))+"/"+String(year(tm));
  return tim; 
}
void blinkled(int t){
  for(int i=0; i<t;i++){
  digitalWrite(2, LOW);
  delay(50);
  digitalWrite(2, HIGH);
  delay(50);
  }
}

