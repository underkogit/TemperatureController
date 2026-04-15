#include "includes.h"

ST7735Display disp(10, 9, 8);
Thermistor th(THERMISTOR_PIN, 10, 10000.0, 10000.0, 3950.0, 25.0);
float temperature = 0;
bool isUpdateUI = false;
void setup()
{

  Serial.begin(9600);
  analogReference(DEFAULT);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);

  disp.begin();
  disp.clearScreen();

  delay(500);
}
void view_ui()
{
  disp.clearScreen();
  pos_x = POSXDEF;
  disp.drawString("Temperature", 1, 5, pos_x, ST77XX_WHITE);
  pos_x += 10;
  disp.drawInt(temperature, 1, 5, pos_x, ST77XX_WHITE);
  pos_x += 10;
  disp.drawString("Dasd Temperature", 1, 5, pos_x, ST77XX_WHITE);
  pos_x += 10;
  disp.drawInt(100, 1, 5, pos_x, ST77XX_WHITE);
}

void loop()
{

  temperature = th.readTemperature();

  if (abs(temperature - th.last_temperature) > 0.1)
  {
    th.last_temperature = temperature;
    isUpdateUI = true;
  }



  
  if (isUpdateUI)
  {
    view_ui();
    isUpdateUI = false;
  }
}