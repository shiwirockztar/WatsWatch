#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>
#include <Adafruit_SSD1306.h>

class WiFiManager {
public:
    WiFiManager(const char* ssid, const char* password, Adafruit_SSD1306* display);
    void connect();
    bool isConnected();
    String getIP();
private:
    const char* _ssid;
    const char* _password;
    Adafruit_SSD1306* _display;
    void printConnecting();
    void printConnected();
    int _nd = 0;
    String _dots = "";
};

#endif // WIFI_MANAGER_H
