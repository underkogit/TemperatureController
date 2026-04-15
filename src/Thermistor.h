// Thermistor.h
#ifndef THERMISTOR_H
#define THERMISTOR_H

#include <Arduino.h>
#include <math.h>

class Thermistor
{
private:
    uint8_t pin;
    int numSamples;
    float seriesResistor;
    float nominalResistance;
    float bCoefficient;
    float temperatureNominal;
    float temperatureNominal;
public:
    float temperature;
    float last_temperature;
    float resistance;

    Thermistor(uint8_t analogPin,
               int samples = 10,
               float seriesR = 10000.0,
               float nominalR = 10000.0,
               float bCoef = 3950.0,
               float tempNominal = 25.0)
        : pin(analogPin),
          numSamples(samples),
          seriesResistor(seriesR),
          nominalResistance(nominalR),
          bCoefficient(bCoef),
          temperatureNominal(tempNominal),
          temperature(0.0f),
          last_temperature(0.0f),
          resistance(0.0f) {}

    float readTemperature()
    {
        int samplesArr[NUMSAMPLES_MAX];
        int n = numSamples;
        if (n <= 0)
            n = 1;
        if (n > NUMSAMPLES_MAX)
            n = NUMSAMPLES_MAX;

        for (int i = 0; i < n; i++)
        {
            samplesArr[i] = analogRead(pin);
            delay(10);
        }

        float average = 0.0f;
        for (int i = 0; i < n; i++)
            average += samplesArr[i];
        average /= n;

        if (average <= 0.0f)
            average = 1.0f;

        resistance = seriesResistor * ((1023.0f / average) - 1.0f);

        float steinhart;
        steinhart = resistance / nominalResistance;
        steinhart = log(steinhart);
        steinhart = steinhart / bCoefficient;
        steinhart = steinhart + 1.0f / (temperatureNominal + 273.15f);
        steinhart = 1.0f / steinhart;
        steinhart = steinhart - 273.15f;

        temperature = steinhart;
        return temperature;
    }

    static const int NUMSAMPLES_MAX = 50;
};

#endif // THERMISTOR_H
