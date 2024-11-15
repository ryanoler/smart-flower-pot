#include "Particle.h"
#include <Adafruit_MQTT.h>
#include "Adafruit_MQTT/Adafruit_MQTT_SPARK.h"
#include "Adafruit_MQTT/Adafruit_MQTT.h"

#include "IotClassroom_CNM.h"
#include "Adafruit_BME280.h"
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"
#include "Grove_Air_Quality_Sensor.h"
#include "IoTTimer.h"
#include "credentials.h"

TCPClient TheClient; 
Adafruit_MQTT_SPARK mqtt(&TheClient,AIO_SERVER,AIO_SERVERPORT,AIO_USERNAME,AIO_KEY);
Adafruit_MQTT_Subscribe webButton = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/buttonvalue"); 
Adafruit_MQTT_Publish bmeHumidity = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/humidity");
Adafruit_MQTT_Publish bemAirpressure = Adafruit_MQTT_Publish(&mqtt,AIO_USERNAME"/feeds/airpressure");
Adafruit_MQTT_Publish soilPin = Adafruit_MQTT_Publish(&mqtt,AIO_USERNAME"/feeds/analogsoilPin");
Adafruit_MQTT_Publish bemTemp =Adafruit_MQTT_Publish(&mqtt,AIO_USERNAME"/feeds/temperature");
Adafruit_MQTT_Publish AirQuality = Adafruit_MQTT_Publish(&mqtt,AIO_USERNAME"/feedsairqualitysensor");

Adafruit_BME280 bme;
const int OLED_RESET=-1;
Adafruit_SSD1306 display(OLED_RESET);
IoTTimer pumpTimer;
float tempC;
float pressPA;
float humidRH;
float tempF;
float inHg;
bool status;
uint CurrentTime;
uint last;
bool webButtonState;
AirQualitySensor sensor(A0);
int analogsoilPin=A1;
int Val;
int soilPinVal;
void MQTT_connect();
bool MQTT_ping();
SYSTEM_MODE(SEMI_AUTOMATIC);




SYSTEM_THREAD(ENABLED);


SerialLogHandler logHandler(LOG_LEVEL_INFO);


void setup() {

mqtt.subscribe(&webButton);
Serial.begin(9600);
waitFor(Serial.isConnected,10000);
soilPinVal = analogRead(analogsoilPin);
pinMode(analogsoilPin,INPUT);
display.begin(SSD1306_SWITCHCAPVCC, 0x3c);
display.clearDisplay();
display.setTextSize(1);
display.setTextColor(WHITE);
display.setCursor(0,0);
display.display();
status = bme.begin(0x76) ;
if(status==false){
display.printf( "BME280 at address %02x failed to start",0x76);
}   
   Serial.begin(9600);
    while (!Serial);

    Serial.println("Waiting sensor to init...");
    display.println("Waiting sensor to init...");
    display.display();

    delay(20000);

    if (sensor.init()) {
        Serial.println("Sensor ready.");
        display.printf("Sensor ready.");
    } else {
        Serial.println("Sensor ERROR!");
        display.printf("Sensor ERROR!");
    }
      Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(100))) {
    if(subscription == &webButton) {
      webButtonState = atoi((char *)webButton.lastread);
      Serial.printf("button value %i\n",webButtonState);
      digitalWrite(D5,webButtonState);
              
    }
  }
pinMode(D5,OUTPUT);
  
}
  void loop() {
  display.setCursor(0,0);
  int quality = sensor.slope();

    Serial.println("Sensor value: ");
    Serial.println(sensor.getValue());
    display.display();
    display.printf("Sensor value: ");
   
    

    if (quality == AirQualitySensor::FORCE_SIGNAL) {
        Serial.println("High pollution! Force signal active.");
        display.printf("High pollution! Force signal active.");
    } else if (quality == AirQualitySensor::HIGH_POLLUTION) {
        Serial.println("High pollution!");
        display.printf("High pollution!");
    } else if (quality == AirQualitySensor::LOW_POLLUTION) {
        Serial.println("Low pollution!");
        display.printf("Low pollution!");
    } else if (quality == AirQualitySensor::FRESH_AIR) {
        Serial.println("Fresh air.");
        display.printf("Fresh air.");
        
    }

    delay(1000);

    display.clearDisplay();
    display.setCursor(0,0);
    soilPinVal = analogRead(analogsoilPin);
    tempC=bme.readTemperature();
    pressPA=bme.readPressure();
    humidRH=bme.readHumidity();
    
    
    //display.clearDisplay();
    
    // convert tempc to tempf
    tempF=9/5.0*tempC+32;
    // convert pascals to inhg
    inHg=pressPA*2.03602;
    
    
    
    

    Serial.printf("temp Value = %0.1f --- pressure = %0.1f humidity=%0.1f---  moisture = %i\n",tempF,pressPA,humidRH,soilPinVal); 
    display.printf("temp Value = %0.1f --- pressure = %0.1f humidity=%0.1f---moisture = %i\n",tempF,pressPA,humidRH,soilPinVal); 
    display.display();
    delay(10000);
    //display.clearDisplay();

    if(soilPinVal>=3078){
        digitalWrite(D5,HIGH);
        pumpTimer.startTimer(1000);
    }

   
    else{
    digitalWrite(D5,LOW);
    }
    if(pumpTimer.isTimerReady()){
        digitalWrite(D5,LOW);
    }
    
    
    //Serial.printf("SoilPinVal=%i,\n is adequate.",soilPinVal);
    //display.printf("SoilPinVal=%i,\n is adequate.",soilPinVal);
    //display.display();
       
    //display.clearDisplay();
   
    

 
}
