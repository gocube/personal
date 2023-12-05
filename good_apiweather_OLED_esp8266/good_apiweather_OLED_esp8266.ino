/*Automatic get location (by IP) => Auto get weather information
Device: SSD1306 OLED, ESP8266

Connection
OLED    ESP
GND     GND
VDD     3V3
SCK     D1
SDA     D2

20230919 1517: Project pass QA. Can build base on this */


#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <TimeLib.h>
//#include <C:\Users\leung\Documents\Arduino\libraries\Adafruit_GFX_Library\Fonts\Tiny3x3a2pt7b.h>
#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels
#define OLED_RESET -1     // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//Control Panel Here//
const char* ssid = "UPC318E5A1";
const char* password = "kWbmpj8Akhha";
const char* city = "";
const char* apiKey = "c4788cf2ee27f81aed6fb861aa4d0d10";


//Var to be used
float temperature =0 ;
unsigned long unixTimestamp = 0;
unsigned long timezone_offset = 0;
float mintemp = 0;
float maxtemp = 0;
float hum = 0;
int wind_spd = 0;
int wind_hdg = 0;
const char* tips = "";

void setup() {
  Serial.begin(115200);
  const char* wifistate= "";
  // Init Screen 
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  //MUST HAVR
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);  }
  display.clearDisplay();
  //display.setFont(&Tiny3x3a2pt7b);    Activate in case need to change font
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);


  //  Connect to WIFI , check connectivity each 1k ms
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(2000);
      wifistate = "Connecting";
      display.setCursor(5,5);
      display.print("WELCOME!");
      display.setCursor(5,25);
      display.print(wifistate);
      display.display();
      Serial.println("Connecting to WiFi..."); }
        Serial.println("Connected to WiFi");
        wifistate = "Connected";
        display.clearDisplay();
        display.setCursor(5,5);
      display.print("WELCOME!");
      display.setCursor(5,25);  
      display.print(wifistate);
      display.display();
    
}

void loop() {
  
  // Step 1: Get Current City By IP
  WiFiClient client;
  HTTPClient http;

  http.begin(client, "http://ip-api.com/json/?fields=status,city");
  http.GET();

  String payload0 = http.getString();
  DynamicJsonDocument jsonDoc(1024);  // Adjust the size as needed
  DeserializationError error = deserializeJson(jsonDoc, payload0);
  String city = jsonDoc["city"];
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
    DynamicJsonDocument jsonDoc(1024);  // Adjust the size as needed
    DeserializationError error = deserializeJson(jsonDoc, payload);

    // Extract weather information if there is no error
    if (!error) {
      //Handling JSON Columns
      temperature = jsonDoc["main"]["temp"];
      mintemp = jsonDoc["main"]["temp_min"];
      maxtemp = jsonDoc["main"]["temp_max"];
      hum = jsonDoc["main"]["humidity"];
      wind_spd = jsonDoc["wind"]["speed"];
      wind_hdg = jsonDoc["wind"]["deg"];
      String description = jsonDoc["weather"][0]["description"];
      tips = jsonDoc["weather"][0]["description"];
      unixTimestamp = jsonDoc["dt"];
      timezone_offset = jsonDoc["timezone"];


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

  //Update time processor UNIX => MM/DD HH:MM
  setTime(unixTimestamp+timezone_offset);
  char formattedTime[16];
  snprintf(formattedTime, sizeof(formattedTime), "%02d/%02d %02d:%02d", month(), day(), hour(), minute());

  //Display Section
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(1,1);

  display.print("Cur.Temp:");
  display.print(temperature);
  display.println(" C");


  display.print(mintemp);
  display.print(" C -- ");
  display.print(maxtemp);
  display.println(" C");

  display.println("");
  display.print(hum);
  display.print("% ");
  display.print(wind_hdg);
  display.print("'");
  display.print(wind_spd*3.6);
  display.println("kmh");


  display.print(tips);  
  
  
  
  display.setCursor(1,55);
  display.print(city+" ");
  display.print(formattedTime);
    
  display.display(); 
  delay(60000);  // Wait for 1 minute before making the next request
}