/*Copyright Peter Leung 2023*//*
Runtime Enviroment: UNO
Sensor: AHT10 , TFT 2.6 LCD SHIELD
20230913 1211: AHT10 + TFT works fine
WARNING: EASY LOOSE CABLE ==> SCCREEN FREEZE
Connection: 
TFT       UNO
LCD_RST   D10
LCD_CS    A3
LCD_RS    A2
LCD_WR    A1
LCD_RD    A0
GND       GND(breadboard shared)
5V        5V (breadboard shared)
LCD_D2    D2
LCD_D3    D3
LCD_D4    D4
LCD_D5    D5
LCD_D6    D6
LCD_D7    D7  
LCD_D0    D8
LCD_D1    D9


AHT10   UNO
GND     GND(breadboard shared)
VIN     5V (breadboard shared)
SCL     A5  
SCA     A4


20230915 1628: Tested and work EXCELLENT. Can build base on this.
*/
#include <Wire.h>
#include <AHTxx.h>
#include <Adafruit_GFX.h>
#include <Adafruit_TFTLCD.h>

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
#define LCD_RESET 10    //Transfer to D10 from analog

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
#define LIGHTBLUE 0x3f7f

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
AHTxx aht10(AHTXX_ADDRESS_X38, AHT1x_SENSOR);
int number = 0; 
/*For optional runttime second use*/
unsigned long previousMillis = 0; // Stores the previous millis() value
unsigned long interval = 1000;    // Interval in milliseconds (1 second)
int count = 0;   


void setup() {



  tft.reset();

  uint16_t identifier = tft.readID();

  tft.begin(identifier);

  
  tft.fillScreen(BLACK);
    tft.setCursor(1,5);
    tft.setTextColor(WHITE);
    tft.setTextSize(2);
    tft.println("PETER WXR STATION");
    tft.fillRect(0, 25, 350,5, WHITE);
    
  int barX = 30;
  int barY = 250;
  int barWidth = 200;
  int barHeight = 20;
uint16_t startColor = RED;
  uint16_t endColor = BLUE;

  // Draw the gradient rectangular bar
  drawGradientRect(barX, barY, barWidth, barHeight, startColor, endColor);


}
  
  
  
}
void loop() {
  float tmp = aht10.readTemperature();
  float hum = aht10.readHumidity();
  tft.setTextColor(RED);
  tft.setTextSize(4);


  tft.fillRoundRect(15, 50, 170, 50, 7, WHITE);
  tft.setCursor(30, 60);
  tft.print(aht10.readTemperature());
  tft.setCursor(160, 60);
  tft.print("C");

  tft.fillRect(190, 77, 100, 15, BLACK);    //Temperautr word background

  if(tmp>=31) {
    tft.fillCircle(210, 60, 13, RED);
    tft.setTextSize(1);
    tft.setCursor(195, 80);
    tft.print("V. Hot");
  } else if (tmp>=27 && tmp<31) {
    tft.fillCircle(210, 60, 13, ORANGE);
    tft.setTextSize(1);
    tft.setCursor(195, 80);
    tft.print("Hot");
  } else if (tmp>=23 && tmp<27) {
    tft.fillCircle(210, 60, 13, GREEN);
    tft.setCursor(195, 80);
    tft.setTextSize(1);
    tft.print("Cozy");
  } else if (tmp>=17 && tmp<23) {
    tft.fillCircle(210, 60, 13, LIGHTBLUE);
    tft.setCursor(195, 80);
    tft.setTextSize(1);
    tft.print("COOL");
    } else {
    tft.fillCircle(210, 60, 13, BLUE);
    tft.setCursor(195, 80);
    tft.setTextSize(1);
    tft.print("COLD");
    }
  

  // **********************************
  tft.setTextSize(4);
  tft.fillRoundRect(15, 120, 170, 50, 7, WHITE);
  tft.setCursor(30,130 );
  tft.print(aht10.readHumidity()); 
  tft.setCursor(160, 130);
  tft.print("%");

  tft.fillRect(190, 147, 100, 15, BLACK); //Humduity word background 
  if(hum>=60) {
  tft.fillCircle(210, 130, 13, mycolor);
  tft.setCursor(195, 150);
  tft.setTextSize(1);
  tft.print("Wet");
  } else if (hum>=40 && hum<60) {
  tft.fillCircle(210, 130, 13, BLUE);
  tft.setCursor(195, 150);
  tft.setTextSize(1);
  tft.print("Cozy");
  } else {
  tft.fillCircle(210, 130, 13, mycolor);
  tft.setCursor(195, 150);
  tft.setTextSize(1);
  tft.print("Dry");
  };
  delay(3000);

  tft.setTextSize(2);



  /* ****************/
unsigned long currentMillis = millis();
if (currentMillis - previousMillis >= interval) {
  count++;
  tft.fillRoundRect(20,190,100,50,7,WHITE);
  tft.setCursor(20, 200);
  tft.println("Operated:");  //runtime
  tft.setCursor(40, 220);
  tft.println(count);
}


}
