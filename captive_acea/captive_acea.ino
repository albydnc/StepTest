/* DATA-LOGGER Step Test basato su ESP8266
 *  
 *  Materiali:
 *  Il SoC utilizzato è un ESP-12E su scheda NodeMCU
 *  Il Modem utilizzato è SIMCOM SIM900A
 *  Il Convertitore A/D è PCF8591T
 *  
 *  Utilizzo:
 *  Inserire la configurazione in questa pagina indicando
 *  UPurl e CONFurl, debug e selezione sim.
 *  
 *  Release Log:
 *  15/05/17
 *  - Aggiunta funzione primitiva per il risparmio energetico
 *  - Aggiornamento configurazione automatico
 *  */
//INCLUSIONE librerie
#include <TimeLib.h>
#include <Time.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <Wire.h>
#include <SoftwareSerial.h>

//Definire per Debug Seriale
//#define DEBUG 

// definire selezione sim
//#define VODAFONE
#define TIM

//DEFINIZIONE COSTANTI E VARIABILI
SoftwareSerial gprs(4,5); //RX:D2,TX:D1//SD1/SD2
const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);
DNSServer dnsServer;
ESP8266WebServer webServer(80);
String UPurl = "http://precursori.altervista.org/php_test/log.php?misura1=";
String CONFurl = "http://precursori.altervista.org/php_test/config.txt";
int conf[5]; // file conf: T misura, T upload, dopo, Tmisura,Tupload
time_t DataI, DataO,lastconf,confver;
String lastTime = "";
byte p =0; // puntatore per le misure
unsigned long last = 0; // time per le misure
float lettura0=0;
float lettura1=0;
int fondo1, fondo2;
String unit;
String stato;
bool wifi=true;
                                                                      //SETUP periferica, Eseguito all'avvio della periferica
void setup() {
  ESP.wdtDisable();
  pinMode(2, OUTPUT);
  blinkled(15);
  #ifdef DEBUG
  Serial.begin(9600);
  #endif
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP("Step-Test","acea10064");
  dnsServer.start(DNS_PORT, "*", apIP);
  webServer.onNotFound([]() {
    webServer.send(200, "text/html", HtmlPage());
  });
  webServer.on ( "/wifioff", []() {
    webServer.send ( 200, "text/plain", "wifi off" );
    delay(500);
    WiFi.mode(WIFI_OFF);
    wifi = false;
    #ifdef DEBUG
    Serial.println("WiFi Spento");
    #endif
  } );
  webServer.begin();
  digitalWrite(2, HIGH);
  gprs.setTimeout(10000);
  gprs.begin(9600);
  gprs.println("AT\r");
  while(!gprs.find("OK"));
  cellConnect();
  digitalWrite(2, LOW);
  delay(500);
  for(int i = 0; i <3; i++){
    if(!confGet(CONFurl,false))
    { stato = "Configurazione Fallita, Riavviare!";
    conf[0]=10; conf[1]=30; conf[2]=1;conf[3]=10; conf[4]= 30; //default
    }else{break;}
  }
  //setSyncInterval(1800);
  Wire.begin(12,13); //SDA:D6,SCL:D7
  #ifdef TIM
  setSyncProvider(setDateTimeWeb); //per sim con problemi di configurazione orologio
  #endif
  #ifdef VODAFONE
  setSyncProvider(setDateTimeGPRS); //per sincronia data ora via ponte radio
  #endif
  #ifdef DEBUG
  Serial.println(timeFormat(now()));
  #endif
  digitalWrite(2, HIGH);
}
                                                                      //FUNZIONE Loop Generale del codice
void loop() {
  if(lastconf < confver) confGet(CONFurl,false);
  if(millis()>1800000 && wifi){
    WiFi.mode(WIFI_OFF);
    #ifdef DEBUG
    Serial.println("WiFi Spento");
    #endif
    wifi=false;
  }
  if(now()>=DataI &&now()<=DataO || DataI==DataO){
    misura(conf[0],conf[1]);
  }else{
    if(conf[2]){
      misura(conf[3],conf[4]);
    }else{
      modemOFF();
    }
  }
  dnsServer.processNextRequest();
  webServer.handleClient();
}
