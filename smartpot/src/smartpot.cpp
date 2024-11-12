#include "Particle.h"
#include "Adafruit_BME280.h"
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"
#include "Air_Quality_Sensor.h"
#include "IoTTimer.h"
float tempC;
float pressPA;
float humidRH;
float tempF;
float inHg;
bool status;

int analogsoilPin=A0;
int analogsoilPin=A1;
int Val;
int soilPinVal;
SYSTEM_MODE(SEMI_AUTOMATIC);



SYSTEM_THREAD(ENABLED);


SerialLogHandler logHandler(LOG_LEVEL_INFO);


void setup() {




    
 status = bme.begin(0x76) ;
if(status==false);
Serial.printf( "BME280 at address %02x failed to start",0x76);    
pinMode(D5,OUTPUT);
  
    

    

Serial.begin(9600);
waitFor(Serial.isConnected,10000);
pinMode(analogsoilPin,INPUT);




  

  
 








  
}


void loop() {

digitalWrite(D5,HIGH);
delay(2500);
digitalWrite(D5,LOW);
delay(2500);


soilPinVal = analogRead(analogsoilPin);
Serial.printf("SoilPinVal=%i,\n is adequate.",soilPinVal);
delay(1000);   


 
}
