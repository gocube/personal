/*
20230913 1211: AHT10 + TFT works fine

*/
#include <Wire.h>
#include <AHTxx.h>
#include <Adafruit_GFX.h>
#include <Adafruit_TFTLCD.h>

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
#define LCD_RESET 10

#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF
#define ORANGE 0xFD20
#define GREENYELLOW 0xAFE5
#define NAVY 0x000F
#define DARKGREEN 0x03E0
#define DARKCYAN 0x03EF
#define MAROON 0x7800
#define PURPLE 0x780F
#define OLIVE 0x7BE0
#define LIGHTGREY 0xC618
#define DARKGREY 0x7BEF
#define mycolor 0xFB86

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
AHTxx aht10(AHTXX_ADDRESS_X38, AHT1x_SENSOR);
int number = 0; 
/*For optional runttime second use*/
unsigned long previousMillis = 0; // Stores the previous millis() value
unsigned long interval = 1000;    // Interval in milliseconds (1 second)
int count = 0;   


void setup() {

  Serial.begin(9600);
  Serial.println(F("TFT LCD test"));
  if (!aht10.begin()) {
    Serial.println("Could not find AHT10 sensor!");
    while (1);
  }
#ifdef USE_ADAFRUIT_SHIELD_PINOUT
  Serial.println(F("Using Adafruit 2.4\" TFT Arduino Shield Pinout"));
#else
  Serial.println(F("Using Adafruit 2.4\" TFT Breakout Board Pinout"));
#endif
  Serial.print("TFT size is ");
  Serial.print(tft.width());
  Serial.print("x");
  Serial.println(tft.height());

  tft.reset();

  uint16_t identifier = tft.readID();

  tft.begin(identifier);

  
  tft.fillScreen(BLACK);
    tft.setCursor(1,1);
    tft.setTextColor(RED);
    tft.setTextSize(2);
    tft.println("TEMP:");
       

  
  
  
}
void loop() {
  tft.fillRoundRect(30, 60, 100, 30, 7, WHITE);
  //tft.fillRect(30, 60, 100, 30, WHITE);
  tft.setCursor(65, 70);
  tft.println(aht10.readTemperature());


  tft.fillRoundRect(30, 120, 100, 30, 7, WHITE);
  tft.setCursor(65,130 );
  tft.println(aht10.readHumidity()); 
  delay(3000);


unsigned long currentMillis = millis();
if (currentMillis - previousMillis >= interval) {
  count++;
  tft.fillRoundRect(20,190,100,50,7,WHITE);
  tft.setCursor(20, 200);
  tft.println("Operated:");  //runtime
  tft.setCursor(40, 220);
  tft.println(count);
}
    /*tft.setCursor(1,70);
    tft.setTextColor(RED);
    tft.setTextSize(2);
    double tmp = aht10.readTemperature();
   tft.println(tmp);
  delay(2000);*/

}
