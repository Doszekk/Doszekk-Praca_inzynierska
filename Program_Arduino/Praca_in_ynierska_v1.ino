/* program został napisany przez: 
Adrian Dobosz kierunek: Mechatronika  
*/
// wartoś do testowania w porcie szeregowym 123!123!123!0!0!0!3239!63!


#include <Adafruit_MAX31856.h>
#include <Adafruit_MCP4725.h>
#include <Wire.h>

//Piny analogowe 
#define PIN_Manometr        A3
#define PIN_MomentObrotowy  A0

//Piny cyfrowe wyjciowe     
#define PIN_Grzalka_1       23
#define PIN_Grzalka_2       25
#define PIN_Grzalka_3       27
#define PIN_Elektrozawor_1  33
#define PIN_Elektrozawor_2  35
#define PIN_Elektrozawor_3  37
#define PIN_Obroty          39


//Sześć zmiennych typu float (4 bajty) 3 temperatury cisnienie i moment + jedna zmienna typ uint8_t(1 bajt) obroty silnika i  
//jedna zmienna typu uint16_t(2 bajty) predkosc obrotowa 
#define iloscBajtowDoOdczytu (5*sizeof(float))+(1*sizeof(byte))+(1*sizeof(uint16_t))


//Deklaracja zmienych anologowych
float Temperatura_1, Temperatura_2, Temperatura_3, Cisnienie, Moment;

//Deklaracja zadanych wartosci
uint32_t MAX_Temperatura_1 = -1000, MAX_Temperatura_2 = -1000, MAX_Temperatura_3 = -1000, MAX_Cisnienie = -1000, MAX_Moment = -1000;

uint32_t WlaczenieObrotowSilnika;
uint32_t ObrotySilnika; //poprzez konwerter Adafruit_MCP4725
uint32_t Grzalka_1, Grzalka_2, Grzalka_3, Zawor_1, Zawor_2, Zawor_3;
uint32_t czas = millis();

String strMAX_Temperatura_1;
String strMAX_Temperatura_2;
String strMAX_Temperatura_3;
String strMAX_Cisnienie;
String strMAX_Moment;
String strWlaczenieObrotowSilnika;
String strObrotySilnika;
String strGrzalka_1;
String strGrzalka_2;
String strGrzalka_3;
String strZawor_1;
String strZawor_2;
String strZawor_3;

void OdcztDanychAnalogowych();
void WyslanieWartosciAnalogowych();
void OdczytWartosciZadanych();
void KontrolaTemperatury();
void SterowanieObrotamiSilnika();
void MAX31856_PobierzTemperature();


Adafruit_MAX31856 Termopara_1MAX31856 = Adafruit_MAX31856(2, 7,8,9);
Adafruit_MAX31856 Termopara_2MAX31856 = Adafruit_MAX31856(3, 7,8,9);
Adafruit_MAX31856 Termopara_3MAX31856 = Adafruit_MAX31856(4, 7,8,9);
Adafruit_MCP4725 ObrotySilnika_MCP4725;

void setup() {
  Serial.begin(19200);// Otwiera port UART i ustawia prędkośc przesyłu na 19200bps
  Termopara_1MAX31856.begin();
  delay(500);
  Termopara_2MAX31856.begin();
  delay(500);
  Termopara_3MAX31856.begin();
  delay(500);
  ObrotySilnika_MCP4725.begin(0x60);
  delay(500);
  //Piny analogowe 
  pinMode(PIN_Manometr, INPUT);
  pinMode(PIN_MomentObrotowy, INPUT);
  //Piny cyfrowe wyjciowe  
  pinMode(PIN_Grzalka_1, OUTPUT);
  pinMode(PIN_Grzalka_2, OUTPUT);
  pinMode(PIN_Grzalka_3, OUTPUT);
  pinMode(PIN_Elektrozawor_1, OUTPUT);
  pinMode(PIN_Elektrozawor_2, OUTPUT);
  pinMode(PIN_Elektrozawor_3, OUTPUT);
  pinMode(PIN_Obroty, OUTPUT);
  
  Termopara_1MAX31856.setThermocoupleType(MAX31856_TCTYPE_J);
  Termopara_2MAX31856.setThermocoupleType(MAX31856_TCTYPE_J);
  Termopara_3MAX31856.setThermocoupleType(MAX31856_TCTYPE_J);
  OdczytWartosciZadanych();
}


void loop() {
  if(czas + 250 < millis()){
    czas = millis();
    OdcztDanychAnalogowych();
    KontrolaTemperatury();
    WyslanieWartosciAnalogowych(); // i stanów portów
    SterowanieObrotamiSilnika();
  }
  OdczytWartosciZadanych();
}


void OdcztDanychAnalogowych(){
  MAX31856_PobierzTemperature();
  Cisnienie = analogRead(PIN_Manometr);
  Moment = analogRead(PIN_MomentObrotowy);
}


void WyslanieWartosciAnalogowych(){
  if(Temperatura_1 == -999.9){
    Serial.println("Error");
  }else{
    Serial.println(Temperatura_1);
  }
  if(Temperatura_2 == -999.9){
    Serial.println("Error");
  }else{
    Serial.println(Temperatura_2);
  }
  if(Temperatura_3 == -999.9){
    Serial.println("Error");
  }else{
  Serial.println(Temperatura_3);
  }
  Serial.println(Cisnienie*5000*5/1024);
  Serial.println(Moment*500/1024);
  Serial.println(Grzalka_1);
  Serial.println(Grzalka_2);
  Serial.println(Grzalka_3);
  Serial.println(Zawor_1);
  Serial.println(Zawor_2);
  Serial.println(Zawor_3);
}


void OdczytWartosciZadanych(){
  if(Serial.available() > 0){
    strMAX_Temperatura_1 = Serial.readStringUntil('!');
    strMAX_Temperatura_2 = Serial.readStringUntil('!');
    strMAX_Temperatura_3 = Serial.readStringUntil('!');
    strMAX_Cisnienie = Serial.readStringUntil('!');
    strMAX_Moment = Serial.readStringUntil('!');
    strWlaczenieObrotowSilnika = Serial.readStringUntil('!');
    strObrotySilnika = Serial.readStringUntil('!');
    strGrzalka_1 = Serial.readStringUntil('!');
    strGrzalka_2 = Serial.readStringUntil('!');
    strGrzalka_3 = Serial.readStringUntil('!');
    strZawor_1 = Serial.readStringUntil('!');
    strZawor_2 = Serial.readStringUntil('!');
    strZawor_3 = Serial.readStringUntil('!');
    
    
    if(strMAX_Temperatura_1 == "");else
    MAX_Temperatura_1 = strMAX_Temperatura_1.toInt();
    
    if(strMAX_Temperatura_2 == "");else
    MAX_Temperatura_2 = strMAX_Temperatura_2.toInt();
    
    if(strMAX_Temperatura_3 == "");else
    MAX_Temperatura_3 = strMAX_Temperatura_3.toInt();
    
    if(strMAX_Cisnienie == "");else
    MAX_Cisnienie = strMAX_Cisnienie.toInt();
    
    if(strMAX_Moment == "");else
    MAX_Moment = strMAX_Moment.toInt();
    
    if(strWlaczenieObrotowSilnika == "");else
    WlaczenieObrotowSilnika = strWlaczenieObrotowSilnika.toInt();
    
    if(strObrotySilnika == "");else
    ObrotySilnika = strObrotySilnika.toInt();
    
    if(strGrzalka_1 == "");else
    Grzalka_1 = strGrzalka_1.toInt();
    
    if(strGrzalka_2 == "");else
    Grzalka_2 = strGrzalka_2.toInt();
    
    if(strGrzalka_3 == "");else
    Grzalka_3 = strGrzalka_3.toInt();
    
    if(strZawor_1 == "");else
    Zawor_1 = strZawor_1.toInt();
    
    if(strZawor_2 == "");else
    Zawor_2 = strZawor_2.toInt();
    
    if(strZawor_3 == "");else
    Zawor_3 = strZawor_3.toInt();
    

  }
    
}


void KontrolaTemperatury(){      
  //Temperatura 3
  if(Grzalka_3 == 1 && MAX_Temperatura_3 >= Temperatura_3){
    digitalWrite(PIN_Grzalka_3, HIGH);
    Grzalka_3 = 1;
  }else {
    digitalWrite(PIN_Grzalka_3, LOW);
    Grzalka_3 = 0;
  }

  //Temperatura 2
  if(Grzalka_2 == 1 && MAX_Temperatura_2 >= Temperatura_2){
    digitalWrite(PIN_Grzalka_2, HIGH);
    Grzalka_2 = 1;
  }else {
    digitalWrite(PIN_Grzalka_2, LOW);
    Grzalka_2 = 0;
  }

  //Temperatura 1
  if(Grzalka_1 == 1 && MAX_Temperatura_1 >= Temperatura_1){
    digitalWrite(PIN_Grzalka_1, HIGH);
    Grzalka_1 = 1;
  }else {
    digitalWrite(PIN_Grzalka_1, LOW);
    Grzalka_1 = 0;
  }

  //Elektrozawor 3
  if(Zawor_1 == 1){
    digitalWrite(PIN_Elektrozawor_3, HIGH);
    Zawor_1 = 1;
  }else {
    digitalWrite(PIN_Elektrozawor_3, LOW);
    Zawor_1 = 0;
  }

  //Elektrozawor 2
  if(Zawor_2 == 1){
    digitalWrite(PIN_Elektrozawor_2, HIGH);
    Zawor_2 = 1;
  }else{ 
    digitalWrite(PIN_Elektrozawor_2, LOW);
    Zawor_2 = 0;
  }

  //Elektrozawor 1
  if(Zawor_3 == 1){
    digitalWrite(PIN_Elektrozawor_1, HIGH);
    Zawor_3 = 1;
  }else{
    digitalWrite(PIN_Elektrozawor_1, LOW);
    Zawor_3 = 0;
  }
}

int Temperatura_avr = (Temperatura_1+Temperatura_2+Temperatura_3)/3;

void SterowanieObrotamiSilnika(){
  if(WlaczenieObrotowSilnika == 0 || Moment > MAX_Moment || Cisnienie > MAX_Cisnienie || Temperatura_avr > 100){
     digitalWrite(PIN_Obroty, LOW);
     WlaczenieObrotowSilnika = 0;
  }
  else if(WlaczenieObrotowSilnika == 1){
    digitalWrite(PIN_Obroty, HIGH);
    WlaczenieObrotowSilnika = 1;
  }
    ObrotySilnika_MCP4725.setVoltage(ObrotySilnika, false);
}


void MAX31856_PobierzTemperature(){
  int fault;
  fault = Termopara_1MAX31856.readFault();
  if (fault) {
    Temperatura_1 = -999.9f;
  }else{
    Temperatura_1 = Termopara_1MAX31856.readThermocoupleTemperature();
  }
  fault = Termopara_2MAX31856.readFault();
  if (fault) {
    Temperatura_2 = -999.9f;
  }else{
    Temperatura_2 = Termopara_1MAX31856.readThermocoupleTemperature();
  }
  
  fault = Termopara_3MAX31856.readFault();
  if (fault) {
    Temperatura_3 = -999.9f;
  }else{
    Temperatura_3 = Termopara_3MAX31856.readThermocoupleTemperature();
  }
}



/*
  odczyt temperatury dziala
  komunikacja zamienic '\n' na '!' i bedzie dzialac
  dodac mozliwosc manualnego zalaczenia karzdego z pinow
  obroty dzialaja tylko cos je zmieniato trzeba sprawdzic juz dziala!!!!
  odczyt mometu dziala
  odczyt cisnienia niewiadomo i nie mam jak sprawdzic nawet na uczelni wiec nalezy podac ze dziala
  nie ma kontrolo prawo lewo tylko wloacz wylacz ale to tylko kwestia nazwy a tak to wszystko jest zeby to zrobic
  kontrola temperatury cos szaleje dodac histereze jakas zeby zalaczaly sie jak temp bedze ponizej zadanego a wylaczaly jab bedzei troche ponad a nie na zero
  to samo zrobic z chlodzeniem
  a tak to wszystko dziala
*/











 
