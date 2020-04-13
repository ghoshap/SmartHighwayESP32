
#include <WiFi.h>


#include <Wire.h>       

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16,2);    
#include "DHTesp.h"
 
#define DHTpin 25   
 
DHTesp dht;
#define rainPin 5
#define flamePin 32
#define prox1Pin 4
#define prox2Pin 14
#define photoPin 23
#define LEDPin 22


int CarCount = 0;
String apiKey = "1YWY5TN7MXJW9AWG";                  //  Enter your Write API key from ThingSpeak
const char *ssid = "TNCAP671C37";
const char *pass = "1F0EAB2255";
//const char *ssid = "Aparna";
//const char *pass = "Aparna0123456789";
//const char *ssid =  "7";                                    // wifi ssid and wpa2 key
//const char *pass =  "75757575";
const char* server = "184.106.153.149";
WiFiClient client;

void setup() 
{
    Serial.begin(115200);
    delay(10);

    pinMode(DHTpin, INPUT);
    pinMode(rainPin, INPUT);
    pinMode(flamePin, INPUT);
    pinMode(prox1Pin, INPUT);
    pinMode(prox2Pin, INPUT);
    pinMode(photoPin, INPUT);
    pinMode(LEDPin, OUTPUT);

    Wire.begin(21,19);
lcd.init();  

lcd.backlight(); 

lcd.setCursor(0,0); 
lcd.print("Smart Highway");
delay(1000);
lcd.setCursor(0,1);
lcd.print("Welcome....");
delay(3000);  

lcd.clear();

lcd.setCursor(0,0);
lcd.print("All Alerts will"); 
lcd.setCursor(0,1); 
lcd.print("be Displayed here");

    //Serial.println("8x8 LED Matrix Test");       
    // set the address       
    //matrix.begin(0x70); 
    
    Serial.println("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    dht.setup(DHTpin, DHTesp::DHT11);
 
}
void loop() 
{
  

float t =0;
int rain_value = 1;
int flame_value = 1;
int prox1_value = 1;
int prox2_value = 1;
int photo_value = 1;

rain_value = rain_sensor();
flame_value = flame_sensor();
prox1_value = prox1_sensor();
prox2_value = prox2_sensor();
photo_value = photo_sensor();


if (prox1_value==1)
    CarCount++;

if(prox2_value==1)
    CarCount--;

if(CarCount > 0 && photo_value == 0)
    digitalWrite(LEDPin, HIGH);
else
  digitalWrite(LEDPin, LOW);


 if( CarCount > 9)
    {
      Serial.println("Traffic!!!!!");
      lcd.clear();
      lcd.print("Traffic!!!!!");
      } 

delay(1000);
 
 int rain_sensor() {
  // read the input on digital pin 5:
  int sensorValue = digitalRead(rainPin);
  if(sensorValue == 0){
    Serial.println(" - It's raining love");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Wet Roads"); 
    lcd.setCursor(0,1); 
    lcd.print("Go Slow...");
    return 0;
  }
  else {
    Serial.println(" - It's dry");
    //Scroll_Text("DRY....");
    return 1;
  }
  delay(500);
}
 
 
  while(dht.getStatusString() == "TIMEOUT")
  {
    humidity = dht.getHumidity();
    temperature = dht.getTemperature();
    }
 
  Serial.println(dht.getStatusString());
  


 if (client.connect(server,80))                                 //   "184.106.153.149" or api.thingspeak.com
    {  
      Serial.println("connected to thingspeak");
      String cmd = "AT+CIPSTART=\"TCP\",\"";                  // TCP connection
      cmd += "184.106.153.149";                               // api.thingspeak.com
      cmd += "\",80";
      String getStr = "GET https://api.thingspeak.com/update?api_key=";   // prepare GET string
      getStr += apiKey;
  
      getStr +="&field1=";
      getStr += String(prox1_value);
      getStr +="&field2=";
      getStr += String(temperature);
      getStr +="&field3=";
      getStr += String(rain_value);
      getStr += "&field4=";
      getStr += String(flame_value);
      getStr += "&field5=";
      getStr += String(prox2_value);
      getStr += "&field6=";
      getStr += String(humidity);
      getStr += "&field7=";
      getStr += String(photo_value);
      getStr += "&field8=";
      getStr += String(CarCount);
      
      // ...
      getStr += "\r\n\r\n";

      // send data length
      cmd = "AT+CIPSEND=";
      cmd += String(getStr.length());
      client.print(getStr);
      Serial.print("Proximity 1 Sensor value: ");
      Serial.println(prox1_value);
      Serial.print("Temperature: ");
      Serial.print(temperature);
      Serial.println(" C");
      Serial.print("Humidity: ");
      Serial.println(humidity);
      Serial.print("Flame Sensor value: ");
      Serial.println(flame_value);
      Serial.print("Rain Sensor value: ");
      Serial.println(rain_value);
      Serial.print("Photocell Sensor value: ");
      Serial.println(photo_value);
      Serial.print("Proximity 2 Sensor value: ");
      Serial.println(prox2_value);
      Serial.print("Number of cars in the highway right now : ");
      Serial.println(CarCount);
      //lcd.print(String(CarCount));
      
      delay(300); 
                               
      Serial.println("%. Send to Thingspeak.");
}
    client.stop();
    Serial.println("Waiting...");
    delay(10000);
}




int prox1_sensor()
{
  int sensorValue = digitalRead(prox1Pin);
  //CarCount++;
  return sensorValue;
  } 
int prox2_sensor()
{
  int sensorValue = digitalRead(prox2Pin);
  //CarCount--;
  return sensorValue;
  }

 int photo_sensor()
 {
  int sensorValue = digitalRead(photoPin);
  return sensorValue;
  }

  

