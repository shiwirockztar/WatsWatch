#include "manager/DisplayManager.h"

DisplayManager::DisplayManager(Adafruit_SSD1306* display) : _display(display) {}

void DisplayManager::showInicio(int sel) {
    _display->clearDisplay();
    _display->setTextSize(2);
    _display->setTextColor(WHITE);
    _display->setCursor(28, 0);
    _display->println("Inicio");
    if(sel == 0) {
        _display->fillRoundRect(30, 18, 65, 20, 2, WHITE);
        _display->setTextColor(BLACK);
        _display->setCursor(35, 20);
        _display->println("ESP 1");
        _display->setTextColor(WHITE);
        _display->setCursor(35, 40);
        _display->println("ESP 2");
    } else {
        _display->setTextColor(WHITE);
        _display->setCursor(35, 20);
        _display->println("ESP 1");
        _display->fillRoundRect(30, 38, 65, 20, 2, WHITE);
        _display->setTextColor(BLACK);
        _display->setCursor(35, 40);
        _display->println("ESP 2");
    }
    _display->display();
}

void DisplayManager::showESP1(float pot, int ledState) {
    _display->clearDisplay();
    _display->setTextSize(2);
    _display->setTextColor(WHITE);
    _display->setCursor(28, 0);
    _display->println("Esp 1");
    _display->setTextSize(1);
    _display->setCursor(7, 30);
    _display->print("Pot:");
    _display->println(pot);
    _display->setCursor(7, 50);
    _display->print("Led: ");
    _display->setTextColor(BLACK);
    _display->fillRoundRect(35, 48, 20, 10, 2, WHITE);
    if(ledState == 0) {
        _display->print("OFF");
        _display->drawCircle(90, 40, 15, WHITE);
    } else {
        _display->print("ON");
        _display->fillCircle(90, 40, 15, WHITE);
    }
    _display->display();
}

void DisplayManager::showESP2(float temp, int ledState) {
    _display->clearDisplay();
    _display->setTextSize(2);
    _display->setTextColor(WHITE);
    _display->setCursor(28, 0);
    _display->println("Esp 2");
    _display->setTextSize(1);
    _display->setCursor(7, 30);
    _display->print("Temp:");
    _display->print(temp);
    _display->setCursor(7, 50);
    _display->print("Led: ");
    _display->setTextColor(BLACK);
    _display->fillRoundRect(35, 48, 20, 10, 2, WHITE);
    if(ledState == 0) {
        _display->print("OFF");
        _display->drawCircle(90, 40, 15, WHITE);
    } else {
        _display->print("ON");
        _display->fillCircle(90, 40, 15, WHITE);
    }
    _display->display();
}
