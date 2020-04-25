#define BLYNK_PRINT Serial
#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

const int relay = D2;
const int oneWireBus = D1;     // GPIO where the DS18B20 is connected to
const int wifiLed = D3;
// char auth[] = "OXHw5XoEniLm16AjOSi7qa2AMyvKLW9g";   //masukan token anda
// char auth[] = "N58tGgJMm8Rz4XJT2OVQ3fNsQ3uDYQm7"; //anung
char auth[] = "H8gN-DoO6nM9FUVkDW-1OQEbKOwMTQqy"; //bdmn
char ssid[] = "ardinista";            //nama wifi
char pass[] = "ardiasta";             //password wifi

#define alamatSuhu V0                     //penyimpanan alamat suhu V0-V255
#define alamatRelay V1                     //penyimpanan alamat suhu V0-V255
BlynkTimer timer;
BlynkTimer timerWifi;
OneWire oneWire(oneWireBus);// Setup a oneWire instance to communicate with any OneWire devices
DallasTemperature sensors(&oneWire);// Pass our oneWire reference to Dallas Temperature sensor 

void event(){
  float temperatureC;
  for (int i = 0; i < 10; i++){
    sensors.requestTemperatures(); 
    temperatureC = sensors.getTempCByIndex(0);
    if(temperatureC>10)break;  
  }
  Blynk.virtualWrite(alamatSuhu,temperatureC);
  Serial.println(temperatureC);
}
void kedip(){
  static uint8_t led;
  if (++led<3){
    digitalWrite(wifiLed,1);
  }
  else{
    digitalWrite(wifiLed,0);
  }
  if(led==6)led=0;
}
BLYNK_WRITE(alamatRelay)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  digitalWrite(relay,pinValue);
}
void setup() {
  Serial.begin(9600);// Start the Serial Monitor
  sensors.begin();// Start the DS18B20 sensor

  pinMode(relay,OUTPUT);
  pinMode(wifiLed,OUTPUT);
  digitalWrite(wifiLed,0);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000L, event);
  timerWifi.setInterval(100L, kedip);
}

void loop() {
  Blynk.run();
  timer.run(); // Initiates BlynkTimer
  timerWifi.run(); // Initiates BlynkTimer
}


