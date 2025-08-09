#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Adafruit_SSD1306.h>
#include <Arduino.h>

class DisplayManager {
public:
    DisplayManager(Adafruit_SSD1306* display);
    void showInicio(int sel);
    void showESP1(float pot, int ledState);
    void showESP2(float temp, int ledState);
private:
    Adafruit_SSD1306* _display;
};

#endif // DISPLAY_MANAGER_H
