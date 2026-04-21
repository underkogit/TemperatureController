#include "includes.h"

ST7735Display disp(10, 9, 8);
Thermistor th(THERMISTOR_PIN, 10, 10000.0, 10000.0, 3950.0, 25.0);

float temperature = 0;
float max_temperature = 0;
bool isUpdateUI = false;
unsigned long lastTempUpdate = 0;
bool isFire = false;
int pos_x = 5;

void addToMax(float delta)
{
  max_temperature += delta;
  if (max_temperature < 0)
    max_temperature = 0;

  isUpdateUI = true;
}

void setup()
{
  Serial.begin(9600);

  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);

  pinMode(RELAY1, OUTPUT);
  digitalWrite(RELAY1, HIGH);

  analogReference(DEFAULT);

  disp.begin();
  disp.clearScreen();

  delay(500);
}

void append_pos_text()
{
  pos_x += 10 + GLOBAL_TEXT_SIZE * 1.2;
}

void view_ui()
{
  disp.clearScreen();
  pos_x = POSXDEF;

  disp.drawString("Temperature", GLOBAL_TEXT_SIZE, 5, pos_x, ST77XX_WHITE);
  append_pos_text();

  disp.drawFloat(temperature, GLOBAL_TEXT_SIZE, 5, pos_x, ST77XX_WHITE);

  append_pos_text();
  disp.drawString("MAX Temperature", GLOBAL_TEXT_SIZE, 5, pos_x, ST77XX_WHITE);
  append_pos_text();

  disp.drawFloat(max_temperature, GLOBAL_TEXT_SIZE, 5, pos_x, ST77XX_WHITE);
  append_pos_text();

  disp.drawString(isFire ? "Heating" : "Cooling", GLOBAL_TEXT_SIZE, 5, pos_x, isFire ? ST77XX_RED : ST77XX_GREEN);
}

void loop()
{

  if (millis() - lastTempUpdate > 100)
  {
    bool up = !digitalRead(BTN_UP);
    bool down = !digitalRead(BTN_DOWN);
    lastTempUpdate = millis();
    temperature = th.readTemperature();

    if (up)
    {
      addToMax(APPEND_VALUE);
    }

    if (down)
    {
      addToMax(-APPEND_VALUE);
    }

    if (abs(temperature - th.last_temperature) > TEMPERATURE_UPDATE)
    {
      th.last_temperature = temperature;
      isUpdateUI = true;
    }
    if (max_temperature > 0)
      isFire = max_temperature > temperature;
  }

  digitalWrite(RELAY1, isFire ? LOW : HIGH);

  if (isUpdateUI)
  {
    view_ui();
    isUpdateUI = false;
  }
}