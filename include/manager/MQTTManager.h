#ifndef MQTT_MANAGER_H
#define MQTT_MANAGER_H

#include <PubSubClient.h>
#include <ArduinoJson.h>

class MQTTManager {
public:
    MQTTManager(PubSubClient* client, const char* server, int port);
    void setup(void (*callback)(char*, byte*, unsigned int));
    void reconnect();
    void publishJson(const char* topic, StaticJsonDocument<54>& doc, char* buffer, size_t bufferSize);
    void loop();
private:
    PubSubClient* _client;
    const char* _server;
    int _port;
};

#endif // MQTT_MANAGER_H
