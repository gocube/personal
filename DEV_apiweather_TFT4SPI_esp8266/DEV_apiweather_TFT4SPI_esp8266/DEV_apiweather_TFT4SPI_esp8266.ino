/*
Connection Instruction
LCD            NODEESP8266      DHT
3v3               VCC           VCC
GND               GND           GND
CS                D2
RESET             D3      
DCRS              D4
SDI(MOSI)         D7
SCK               D5 
LED               3V3
                  D1            DATA
*/
#include <SPI.h>
#include "Adafruit_GFX.h"
#include "Adafruit_ST7796S_kbv.h"
#include <string.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <TimeLib.h>
#include "C:\Users\leung\Desktop\icons.h"
#include <DHT.h>
#include <DHT_U.h>
// These are 'flexible' lines that can be changed
#define TFT_CS D2
#define TFT_DC D4
#define TFT_RST D3 // RST can be set to -1 if you tie it to Arduino's reset
Adafruit_ST7796S_kbv tft = Adafruit_ST7796S_kbv(TFT_CS, TFT_DC, TFT_RST);

#define DHTPIN D1
#define DHTTYPE DHT11
DHT_Unified dht(DHTPIN, DHTTYPE);

//Control Panel Here//
const char* ssid = "UPC318E5A1";
const char* password = "kWbmpj8Akhha";
const char* city = "";
auto city_done = 0;
const char* ip ="";
const char* apiKey = "c4788cf2ee27f81aed6fb861aa4d0d10";
//Global Var to be used for weather and times
double temperature =0 ;
unsigned long unixTimestamp = 0;
const char* updateTime ="";
unsigned long timezone_offset = 0;
float mintemp = 0;
float maxtemp = 0;
int hum = 0;
int pressure = 0;
int wind_spd = 0;
int wind_hdg = 0;
const char* tips = "";
unsigned long sunriseTime =0;
unsigned long sunsetTime =0;
unsigned long unixtimeCurrent = 0;
unsigned long abc = 0;
unsigned long previousMillis = 0;
const char* date_time = "";
const long interval = 15000;  // Interval in milliseconds (10 seconds)
// Layout Coordinates
auto maincolumn1_x = 280;


//DHT Sensor Info

/*float sensor_hum = event.relative_humidity;
float sensor_temp = event.temperature;*/

void setup() {
  dht.begin();
  sensor_t sensor; 




  Serial.begin(9600);
  Serial.println("ST7796S_kbv Test!"); 
  tft.begin();
     tft.fillScreen(ST7796S_BLACK);
    tft.setRotation(3);
  const char* wifistate= "";
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(2000);
    wifistate = "Connecting";
    
    tft.setCursor(200, 140);
    tft.setTextSize(2);
    tft.println("WELCOME");
    tft.setCursor(120, 180);
    tft.println("Connecting to WiFi..."); 
    }
  tft.fillScreen(ST7796S_BLACK);
  Serial.println("Connected to WiFi");
  wifistate = "Connected";  
  tft.setCursor(200, 140);
  tft.setTextSize(2);
  tft.println("WELCOME");
  tft.setCursor(190, 180);
  tft.println("Connected"); 

  tft.fillScreen(ST7796S_BLACK);

  delay(2000);
}


void loop() {
  
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis >= interval) {
    // Save the last time the action was performed
    previousMillis = currentMillis;

    // Print "Hello" to the Serial Monitor
    
    mainscreen_layout ();
    CurrentWeatherAPI_dp();
    sundata();
    datastaus();
    clocking();
    clocking_dp();
    CurrentWeatherAPI();
    dht11sensor();
    dht11sensor_dp();


  
  }
}

void mainscreen_layout () {
  tft.drawFastVLine(maincolumn1_x, 0, 500, ST7796S_WHITE);
  tft.drawFastHLine(maincolumn1_x, 190, 500, ST7796S_WHITE);
  tft.drawFastHLine(maincolumn1_x, 260, 500, ST7796S_WHITE);
  
  tft.fillRect(0,0,maincolumn1_x,500,ST7796S_BLACK); //Anti Flick for main section
tft.drawFastHLine(0,210,280,ST7796S_ORANGE);
  tft.drawRGBBitmap(300,1,smallwifi,20,20);
  
}
void sundata() {
  setTime(sunriseTime+timezone_offset);
  char sunriseTime[6];
  snprintf(sunriseTime, sizeof(sunriseTime), "%02d:%02d", hour(), minute());
  setTime(sunsetTime+timezone_offset);
  char sunsetTime[6];
  snprintf(sunsetTime, sizeof(sunsetTime), "%02d:%02d", hour(), minute());
  tft.fillRect(maincolumn1_x+2, 190+2, 300, 260-190-2, ST7796S_BLACK);    //AntiFlick block
  tft.drawRGBBitmap(290-5,200,sunrise,40,40);
    tft.setCursor(290+40-7, 220);
    tft.setTextSize(2);
    tft.print(sunriseTime);
  tft.drawRGBBitmap(380,200,sunset,40,40);
    tft.setCursor(380+34, 220);
    tft.print(sunsetTime);};
void datastaus() {
  tft.fillRect(maincolumn1_x+2,260+2,300,200,ST7796S_BLACK);
  tft.drawRGBBitmap(290-5,266,location,40,40);
  if (city_done==1) {
  tft.setCursor(290+40+30, 266);
  if (String(city).length()<10){
  tft.print(String(city));} else {
    tft.setCursor(290+20,266);
    tft.print(String(city));
  }};
  
  setTime(unixTimestamp+timezone_offset);
  char updateTime[16];
  snprintf(updateTime, sizeof(updateTime), "%02d/%02d %02d:%02d", month(), day(), hour(), minute());
  tft.setCursor(290+40,286+3);
  
  tft.print(updateTime);
};
void clocking () {
  WiFiClient client;
  HTTPClient http;

  http.begin(client, "http://worldtimeapi.org/api/ip");
  http.GET();
  String payload0 = http.getString();
  DynamicJsonDocument jsonDoc(5120);  // Adjust the size as needed
  DeserializationError error = deserializeJson(jsonDoc, payload0);


  unixtimeCurrent = jsonDoc["unixtime"];
  

  
  Serial.println("API Response:");
  Serial.println(payload0);
  Serial.println(unixtimeCurrent);

  http.end();
}
void clocking_dp(){
  setTime(unixtimeCurrent+timezone_offset);
  
  tmElements_t tm;
  breakTime(now(), tm);

  // Print the date in DD/MM format
  Serial.print(tm.Day);
  Serial.print('/');
  Serial.print(tm.Month);
  Serial.print(' ');

  char unixtimeCurrent[16];
  snprintf(unixtimeCurrent, sizeof(unixtimeCurrent), "%02d:%02d",  hour(), minute());


    tft.fillRect( 299, 34, 200, 150, ST7796S_BLACK); //Anti-Flick box
    tft.setCursor(300,34);
      tft.setTextSize(6);
        tft.print(unixtimeCurrent);
     tft.setCursor(315, 100);
      tft.setTextSize(5);
        tft.print(tm.Day);
        tft.print("/");
        tft.print(tm.Month);
        

};
void CurrentWeatherAPI(){
  WiFiClient client;
  HTTPClient http;

  http.begin(client, "http://ip-api.com/json/");
  http.GET();

  String payload0 = http.getString();
  DynamicJsonDocument jsonDoc(5120);  // Adjust the size as needed
  DeserializationError error = deserializeJson(jsonDoc, payload0);
  if (city_done==1) {
  city = jsonDoc["city"];};
  
  city_done = 1;
  ip   = jsonDoc["query"];

  Serial.println("API Response:");
  Serial.println(payload0);
  Serial.println(city);
  http.end();


  //Step 2: Query Weather by Location Obtained in Step1

  http.begin(client, "http://api.openweathermap.org/data/2.5/weather?q=" + String(city) + "&units=metric&appid=" + String(apiKey));

  // Send GET request to the API
  int httpResponseCode = http.GET();
  if (httpResponseCode == 200) {
    String payload = http.getString();
    Serial.println("API Response:");
    DynamicJsonDocument jsonDoc(4000);  // Adjust the size as needed
    DeserializationError error = deserializeJson(jsonDoc, payload);

    // Extract weather information if there is no error
    if (!error) {
      //Handling JSON Columns
      temperature = jsonDoc["main"]["temp"];
      mintemp = jsonDoc["main"]["temp_min"];
      maxtemp = jsonDoc["main"]["temp_max"];
      hum = jsonDoc["main"]["humidity"];
      pressure = jsonDoc["main"]["pressure"];
      wind_spd = jsonDoc["wind"]["speed"];
      wind_hdg = jsonDoc["wind"]["deg"];
      String description = jsonDoc["weather"][0]["description"];
      tips = jsonDoc["weather"][0]["description"];
      unixTimestamp = jsonDoc["dt"];
      timezone_offset = jsonDoc["timezone"];
      sunriseTime = jsonDoc["sys"]["sunrise"];
      sunsetTime  = jsonDoc["sys"]["sunset"];
  
      Serial.print("Temperature (°C): ");
      Serial.println(temperature);
      Serial.print("Description: ");
      Serial.println(description);
      
    } else {
      Serial.println("Failed to parse JSON");
    }
  } else {
    Serial.print("Error: " + httpResponseCode);
  }
  http.end();

}
void CurrentWeatherAPI_dp () {
  tft.drawRGBBitmap(5,10,outdoor,70,70);
  tft.drawRGBBitmap(80,10+10+5,thermo,40,40);
  tft.setCursor(80+30,20+10+5);
  tft.setTextSize(4);
  
  tft.print(temperature);
  tft.print("'C");


  tft.drawRGBBitmap(5,85,down,30,30);
  tft.setCursor(35+5,90);
  tft.setTextSize(2);
  tft.print(mintemp);
  tft.print("'C");
  tft.drawRGBBitmap(100+20+10,85,up,30,30);
  tft.setCursor(160+10,90);
  tft.print(maxtemp);
  tft.print("'C");

  tft.drawRGBBitmap(5,120,waterdrop,30,30);
  tft.setTextSize(3);
  tft.setCursor(35,125);
  tft.print(hum);
  tft.print("%");

  tft.drawRGBBitmap(100+20+10,120,gauge,30,30);
  tft.setCursor(160+10,125);
  tft.print(pressure);
  tft.setTextSize(2);
  tft.print("hpa");

  tft.drawRGBBitmap(5,165,msg,25,25);
  tft.setCursor(35,170);

if (strlen(tips) > 19) {
  tft.setTextSize(1);
}
  tft.print(tips);
  
};

void dht11sensor (){
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  }
  else {
    Serial.print(F("Temperature: "));
    Serial.print(event.temperature);
    Serial.println(F("°C"));
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  }
  else {
    Serial.print(F("Humidity: "));
    Serial.print(event.relative_humidity);
    Serial.println(F("%"));
  } 

};
void dht11sensor_dp () {
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  tft.drawRGBBitmap(5,220, indoor, 40,40);
  tft.setTextSize(4);
  tft.setCursor(80+30,220+5);
  tft.print(event.temperature);
    tft.setTextSize(3);
   tft.setCursor(5,220+45);
   dht.humidity().getEvent(&event);
  tft.print(event.relative_humidity);
  tft.print("%");
};