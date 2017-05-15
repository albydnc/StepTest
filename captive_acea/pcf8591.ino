//DEFINIZIONE variabili relativa al convertitore A/D PCF8591
#define PCF  (0x90>>1)
#define P0  0x40
#define P1  0x41
#define P2  0x42
#define P3  0x43
//FUNZIONE lettura valori analogici dal ADC
byte pcfRead(byte ch){
  Wire.beginTransmission(PCF);
  Wire.write(ch);
  Wire.endTransmission();
  Wire.requestFrom((int) PCF,2);
  int adc=0;
  while(Wire.available()){
    adc=Wire.read();
    adc=Wire.read();
  }
  return adc;
}
//FUNZIONE misura media e conversione in percentuale
void misura(int dm, int du){
  byte nmis = du/dm;
  if(millis()> last){
  lettura0 += (pcfRead(P0)/255.0)*100.00;
  lettura1 += (pcfRead(P1)/255.0)*100.00;
  //lettura += (127/256.0)*100.00;
  p++;
  last= millis()+dm*1000;
  #ifdef DEBUG
  Serial.println("misura: "+String(p));
  #endif
  }
  if(p>=nmis){
    float media = lettura0/p;
    float media1 = lettura1/p; 
    lettura0=0;
    lettura1=0;
    #ifdef DEBUG
    Serial.println("media1: "+String(media)+" media2: "+String(media1));
    #endif
    p=0;
    if(du >= 60) modemON(); // spegne il modem se il periodo di aggiornamento è superiore a 60 sec
    dataSend(media,media1,UPurl,false);
    if(du >= 60) modemOFF();
  }
}
