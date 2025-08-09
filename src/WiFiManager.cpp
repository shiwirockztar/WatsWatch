#include "manager/WiFiManager.h"

WiFiManager::WiFiManager(const char* ssid, const char* password, Adafruit_SSD1306* display)
    : _ssid(ssid), _password(password), _display(display) {}

void WiFiManager::connect() {
    WiFi.begin(_ssid, _password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.println("Conectando...");
        printConnecting();
    }
    Serial.println("Conectado a la red WiFi");
    printConnected();
}

bool WiFiManager::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}

String WiFiManager::getIP() {
    return WiFi.localIP().toString();
}

void WiFiManager::printConnecting() {
    _display->clearDisplay();
    if (_nd > 3) {
        _dots = " ";
        _nd = 0;
    }
    _display->setTextSize(1);
    _display->setTextColor(SSD1306_WHITE);
    _display->setCursor(10, 24);
    _display->print("Conectando");
    _display->print(_dots);
    _dots += ".";
    _nd++;
    _display->display();
}

void WiFiManager::printConnected() {
    _display->clearDisplay();
    _display->setTextSize(1);
    _display->setTextColor(SSD1306_WHITE);
    _display->setCursor(5, 16);
    _display->print("Conectado a:");
    _display->setCursor(5, 28);
    _display->print(_ssid);
    _display->setCursor(5, 40);
    _display->print("Direccion IP:");
    _display->setCursor(5, 52);
    _display->print(WiFi.localIP());
    _display->display();
    delay(4000);
}
