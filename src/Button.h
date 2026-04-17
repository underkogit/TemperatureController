
#ifndef Button_h
#define Button_h

#include <Arduino.h>

class Button
{
private:
    uint8_t _pin;
    bool _lastSteadyState;
    bool _lastFlickerableState;
    unsigned long _lastDebounceTime;
    unsigned long _debounceDelay;
    bool _pressedFlag;
    bool _releasedFlag;

public:
    Button(uint8_t pin)
    {
        _pin = pin;
        _debounceDelay = 50;
        _lastDebounceTime = 0;
        _lastSteadyState = HIGH;
        _lastFlickerableState = HIGH;
        _pressedFlag = false;
        _releasedFlag = false;
    }

    void begin()
    {
        pinMode(_pin, INPUT_PULLUP);
    }

    void setDebounceDelay(unsigned long ms)
    {
        _debounceDelay = ms;
    }

    void update()
    {
        bool currentState = digitalRead(_pin);

        if (currentState != _lastFlickerableState)
        {
            _lastDebounceTime = millis();
            _lastFlickerableState = currentState;
        }

        if ((millis() - _lastDebounceTime) >= _debounceDelay)
        {

            if (currentState != _lastSteadyState)
            {
                _lastSteadyState = currentState;

                if (_lastSteadyState == LOW)
                {
                    _pressedFlag = true;
                }
                else
                {
                    _releasedFlag = true;
                }
            }
        }
    }

    bool isPressed()
    {
        return (_lastSteadyState == LOW);
    }

    bool isReleased()
    {
        return (_lastSteadyState == HIGH);
    }

    bool justPressed()
    {
        if (_pressedFlag)
        {
            _pressedFlag = false;
            return true;
        }
        return false;
    }

    bool justReleased()
    {
        if (_releasedFlag)
        {
            _releasedFlag = false;
            return true;
        }
        return false;
    }
};

#endif