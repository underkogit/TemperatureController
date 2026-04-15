#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

#define THERMISTOR_PIN A1
#define SERIESRESISTOR 100000
#define THERMISTORNOMINAL 100000
#define TEMPERATURENOMINAL 25
#define BCOEFFICIENT 3950
#define NUMSAMPLES 10
#define DELAY_MS 100
#define RELAY_PIN 3

float temperature = 0;
float resistance = 0;

float readTemperature()
{
  int samples[NUMSAMPLES];

  for (int i = 0; i < NUMSAMPLES; i++)
  {
    samples[i] = analogRead(THERMISTOR_PIN);
    delay(10);
  }

  float average = 0;
  for (int i = 0; i < NUMSAMPLES; i++)
  {
    average += samples[i];
  }
  average /= NUMSAMPLES;

  resistance = SERIESRESISTOR * ((1023.0 / average) - 1.0);

  float steinhart;
  steinhart = resistance / THERMISTORNOMINAL;
  steinhart = log(steinhart);
  steinhart = steinhart / BCOEFFICIENT;
  steinhart = steinhart + 1.0 / (TEMPERATURENOMINAL + 273.15);
  steinhart = 1.0 / steinhart;
  steinhart = steinhart - 273.15;

  return steinhart;
}

#define TFT_CS 10
#define TFT_RST 8
#define TFT_DC 9

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

float lastTemp = -999;

void drawInt(double number, uint8_t text_size, int16_t x, int16_t y, uint16_t color)
{
  tft.setTextSize(text_size);
  tft.setRotation(3);
  tft.setTextColor(color);
  tft.setCursor(x, y);
  tft.print(number, 1);
}

void drawString(const String &content, uint8_t text_size, int16_t x, int16_t y, uint16_t color)
{
  tft.setTextSize(text_size);
  tft.setRotation(3);
  tft.setTextColor(color);
  tft.setCursor(x, y);
  tft.print(content);
}

void updateDisplay(float temp)
{

  tft.fillScreen(ST77XX_BLACK);
  uint8_t pos_x = 5;
  drawString("Temperature", 1, 5, pos_x, ST77XX_WHITE);

  pos_x += 10;

  drawInt(temp, 1, 5, pos_x, ST77XX_WHITE);
  pos_x += 10;
  drawString("Dasd Temperature", 1, 5, pos_x, ST77XX_WHITE);
  pos_x += 10;
  drawInt(100, 1, 5, pos_x, ST77XX_WHITE);
}

void setup()
{

  Serial.begin(9600);
  analogReference(DEFAULT);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
  Serial.println("Система запущена");
  delay(500);

  tft.initR(INITR_BLACKTAB);
  tft.fillScreen(ST77XX_BLACK);
}

void loop()
{

  temperature = readTemperature();

  Serial.print("Температура: ");
  Serial.print(temperature);
  Serial.println(" *C");

  if (temperature > 30)
  {
    digitalWrite(RELAY_PIN, HIGH);
  }
  else
  {
    digitalWrite(RELAY_PIN, LOW);
  }

  if (abs(temperature - lastTemp) > 0.1)
  {
    updateDisplay(temperature);
    lastTemp = temperature;
  }

  delay(DELAY_MS);
}