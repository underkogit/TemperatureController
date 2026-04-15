
#ifndef ST7735DISPLAY_H
#define ST7735DISPLAY_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

class ST7735Display
{
private:
    Adafruit_ST7735 tft;
    float lastTemp;
    bool initialized;
    uint8_t csPin;
    uint8_t dcPin;
    uint8_t rstPin;

public:
    ST7735Display(uint8_t cs = 10, uint8_t dc = 9, uint8_t rst = 8)
        : tft(cs, dc, rst), lastTemp(NAN), initialized(false), csPin(cs), dcPin(dc), rstPin(rst) {}

    void begin()
    {

        pinMode(csPin, OUTPUT);
        pinMode(dcPin, OUTPUT);
        pinMode(rstPin, OUTPUT);

        SPI.begin();
        tft.initR(INITR_BLACKTAB);
        tft.setRotation(3);
        tft.fillScreen(ST77XX_BLACK);
        tft.setTextWrap(false);
        initialized = true;
    }

    void clearScreen()
    {
        if (!initialized)
            return;
        tft.fillScreen(ST77XX_BLACK);
    }

    void drawFloat(float number, uint8_t text_size, int16_t x, int16_t y, uint16_t color, uint8_t precision = 1)
    {
        if (!initialized)
            return;
        tft.setTextSize(text_size);
        tft.setRotation(3);
        tft.setTextColor(color);
        tft.setCursor(x, y);
        tft.print(number, precision);
    }

    void drawString(const String &content, uint8_t text_size, int16_t x, int16_t y, uint16_t color)
    {
        if (!initialized)
            return;
        tft.setTextSize(text_size);
        tft.setRotation(3);
        tft.setTextColor(color);
        tft.setCursor(x, y);
        tft.print(content);
    }

    void drawInt(int number, uint8_t text_size, int16_t x, int16_t y, uint16_t color)
    {
        if (!initialized)
            return;
        tft.setTextSize(text_size);
        tft.setRotation(3);
        tft.setTextColor(color);
        tft.setCursor(x, y);
        tft.print(number);
    }
    bool isInit(){
        return initialized;
    }
    void setLastTemp(float temp)
    {
        lastTemp = temp;
    }

    float getLastTemp() const
    {
        return lastTemp;
    }

    Adafruit_ST7735 &getTFT()
    {
        return tft;
    }

   
};

#endif