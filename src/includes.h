#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>
#include "ST7735Display.h"
#include "Thermistor.h"




#define THERMISTOR_PIN A1
#define SERIESRESISTOR 100000
#define THERMISTORNOMINAL 100000
#define TEMPERATURENOMINAL 25
#define BCOEFFICIENT 3950
#define NUMSAMPLES 10
#define RELAY_PIN 3
 

uint8_t pos_x = 5;
#define POSXDEF 5