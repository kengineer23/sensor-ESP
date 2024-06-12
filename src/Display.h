#include <Arduino.h>
#include <TFT_eSPI.h>


#define TFT_MOSI 23
#define TFT_SCLK 18
#define TFT_CS   15   // Chip select control pin
#define TFT_DC   2  // Data Command control pin
#define TFT_RST  4  // Reset pin (could connect to RST pin)

TFT_eSPI tft = TFT_eSPI();

class ili9341{
public:
    ili9341() {
        //initialize the display
        tft.init();
    }

    void begin(){
        //Fill the screen with black colour
        tft.fillScreen(TFT_BLACK);

        //Set text color and background color
        tft.setTextColor(TFT_WHITE, TFT_BLACK);

        //Set the text size
        tft.setTextSize(2);
        tft.setCursor(10,10);
        tft.println("Hello");

        tft.setCursor(10,40);
        tft.println("I am ISAAC");
    }

    void displaydata(float temperature, float humidity, int pm2_5){
        tft.setCursor(10,10);
        tft.println(String(temperature) + " °C");

        tft.setCursor(10,40);
        tft.println(String(humidity) + " %");

        tft.setCursor(10,70);
        tft.println(String(pm2_5) + " µg/m3");
    }
};