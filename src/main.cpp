#include <Arduino.h>
//Librerías necesarias
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <UrlEncode.h>
#include <HTTPClient.h>

// Configuraciónes
#include "../include/config/WiFiConfig.h"
#include "../include/config/WhatsAppConfig.h"
#include "../include/config/MQTTConfig.h"

//Clases de gestión
#include "../include/manager/MQTTManager.h"
#include "../include/manager/WiFiManager.h"
#include "../include/manager/DisplayManager.h"

// bmp Adafruit_BMP085 bmp;
const uint8_t temperatureSet = 25;
bool temperatureSetTriggered = false;

// botón
const uint8_t panicButtonPin = 5;


// Variable global para indicar acceso recibido
volatile bool accesoRecibido = false;
String mensajeAcceso = "";

//Constantes OLED
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 
#define OLED_RESET     -1 //El OLED no tiene botón reset

//Declaración de la pantalla
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
DisplayManager displayManager(&display);

//Datos del WiFi en archivo separado
WiFiManager wifiManager(WIFI_SSID, WIFI_PASSWORD, &display);


//Datos del Broker MQTT en archivo separado

//Cliente MQTT
WiFiClient esp32Client;
PubSubClient mqttClient(esp32Client);
//MQTTManager mqttManager(&mqttClient, MQTT_SERVER, MQTT_PORT);

StaticJsonDocument<54> doc1; //Json para recibir
StaticJsonDocument<54> doc2; //Json para enviar

//Variable para almacenar el mensaje entrate MQTT
String resultS = "";

//Variable para almacenar el Json serializado a enviar
char Json[40];

//# Declarar prototipos de funciones antes de setup
void connectToNetwork();
void checkPanicButton();
void postData (String url);
void callback(char* topic, byte* payload, unsigned int length);

void setup()
{
  Serial.begin(115200);
  pinMode(panicButtonPin, INPUT_PULLUP); 
  connectToNetwork(); 
  //Conectar al WiFi usando la clase WiFiManager
  //wifiManager.connect();
  mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
  mqttClient.setCallback(callback);
    //Configurar broker MQTT usando MQTTManager
  //mqttManager.setup(callback);
}

void reconnect() {
  //Mientras se conecta el broker MQTT
  while (!mqttClient.connected()) {
    Serial.print("Intentando conectarse MQTT...");
    // Es importante asignar un ID al dispositivo conectado al broker
    //si se tienen ID's iguales se puede no recibir ningún mensaje
    //el ID de este ESP32 es "ESPLEO"
    if (mqttClient.connect("ESPLEO")) {
      //Imprimir en el monitor serial conectado
      Serial.println("Conectado");
      //Se suscribe al tópico ESP/main
      mqttClient.subscribe("LEO");
    }
  }
}

void loop(){
  //Verificar conexión con el broker MQTT
   if (!mqttClient.connected()) {
    reconnect();
  }
    //Comprobar si hay mensajes entrates
  mqttClient.loop();

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

// Función para hacer la llamada a la API con POST
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
        //Serial.println("Error.");
    }
    http.end(); // una vez terminada la llamada a la API, finalizamos el cliente
}

void checkPanicButton()
{
  /*Serial.print("panicButton = ");
  Serial.println(digitalRead(panicButtonPin));
  Serial.print("acceso = ");
  Serial.println(accesoRecibido);*/
    if (digitalRead(panicButtonPin) == LOW)
    {
        messageToWhatsApp("Alerta, botón de pánico presionado!");
    }
        // Si se recibió acceso por MQTT, enviar mensaje a WhatsApp
    if (accesoRecibido) {
      messageToWhatsApp(mensajeAcceso);
        //messageToWhatsApp("acceso");
        mensajeAcceso = "";     // Limpiar mensaje
    }
}

/**
 * @brief Callback para recibir mensajes MQTT
 * 
 * Esta función se ejecuta cada vez que se recibe un mensaje en un tópico suscrito.
 * Dependiendo del tópico, se procesan los datos recibidos y se actualizan las variables correspondientes.
 * 
 * @param topic Tópico al que se ha recibido el mensaje
 * @param payload Datos del mensaje recibido
 * @param length Longitud del mensaje recibido
 */
void callback(char* topic, byte* payload, unsigned int length) {
  //Vaciar la variable del Jsonn serializado
  resultS = "";
  //Se convierte el mensaje recibido en String
  for (int i=0;i<length;i++) {
    resultS= resultS + (char)payload[i];
  }
      Serial.println("Mensaje recibido: ");
      Serial.println(resultS);
  //Comando para deserealizar el Json recibido
  
  DeserializationError error = deserializeJson(doc1, resultS);
  if (error) 
  {
    //Verificar si hay un error
    Serial.print(F("deserializeJson() failed with code "));
    Serial.println(error.f_str());
  }
  // Debido a que estamos suscritos a 2 tópicos, se crean condiciones
  //dependiendo del tópico que se recibe el mensaje
  if(String(topic) == "ESP1/Val")
  {
  //Obtener los valores del Json recibido
  //Imprimir en el monitor serial
  Serial.print("Del Tópico ");
  Serial.print(topic);
  }
  
  if(String(topic) == "ESP2/Val")
  {
  //Obtener los valores del Json recibido
  //Imprimir en el monitor serial
  Serial.print("Del Tópico ");
  Serial.print(topic);
  }
    // Si el tópico es "LEO" y el mensaje es "acceso", activar bandera
  if(String(topic) == "LEO")
  {
      Serial.print("Tópico LEO: ");
      Serial.println(resultS);
      mensajeAcceso = resultS;
      accesoRecibido = true;
      /*
      if (resultS == "acceso") {
        accesoRecibido = true;
      }
      */
  }
 // aqui si el topico no es conocido imprima el mensaje de error
  if(String(topic) == "leo")
  {
      Serial.print("Tópico desconocido: ");
      Serial.println(topic);
      Serial.println("Mensaje recibido: ");
      Serial.println(resultS);
  }
}

