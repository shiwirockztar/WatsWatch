#include <Arduino.h>
#include <WiFi.h>
#include<HTTPClient.h>
#include <UrlEncode.h>
#include "esp32_secrets.h"

// bmp Adafruit_BMP085 bmp;
const uint8_t temperatureSet = 25;
bool temperatureSetTriggered = false;

// botón
const uint8_t panicButtonPin = 5;

//# Declarar prototipos de funciones antes de setup
void connectToNetwork();
void checkPanicButton();
void postData (String url);


void setup()
{
  Serial.begin(115200);
  pinMode(panicButtonPin, INPUT_PULLUP); 
  connectToNetwork(); 
}

void loop(){
  //checkTemperature();
  checkPanicButton();
  delay(500);
}


// Funciones de apoyo

void connectToNetwork()
{
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.println("Intentando conectar a la red WiFi");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("Conectando...");
    }
    Serial.println("Conectado a la red WiFi");
}


void messageToWhatsApp (String message)
{
    // adding all number, your api key, your message into one complete url
    String url = "https://api.callmebot.com/whatsapp.php?phone=" + String(PHONE_NUMBER) + "&apikey=" + String(API_KEY) + "&text=" + urlEncode(message);
    postData(url);
}

// https://example.com/search?query=Arduino%20Uno8max_price=20

// Función para hacer la llamada a la API con POS
void postData (String url)
{
    int httpCode;
    HTTPClient http; // declaramos un object HTTPClient
    http.begin(url);
    httpCode = http.POST(url);
if (httpCode == 200) // Verificamos el código de respuesta
{
    Serial.println("Enviado correctamente.");
}
else
{
    Serial.println("Error.");
}
http.end(); // una vez terminada la llamada a la API, finalizamos el cliente
}

void checkPanicButton()
{
    if (digitalRead(panicButtonPin) == LOW)
    {
        messageToWhatsApp("Alerta, botón de pánico presionado!");
    }
}