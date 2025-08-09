#include "manager/MQTTManager.h"

MQTTManager::MQTTManager(PubSubClient* client, const char* server, int port)
    : _client(client), _server(server), _port(port) {}

void MQTTManager::setup(void (*callback)(char*, byte*, unsigned int)) {
    _client->setServer(_server, _port);
    _client->setCallback(callback);
}

void MQTTManager::reconnect() {
    while (!_client->connected()) {
        if (_client->connect("ESPMAIN")) {
            _client->subscribe("ESP1/Val");
            _client->subscribe("ESP2/Val");
            _client->subscribe("leo");
        }
    }
}

void MQTTManager::publishJson(const char* topic, StaticJsonDocument<54>& doc, char* buffer, size_t bufferSize) {
    serializeJson(doc, buffer, bufferSize);
    _client->publish(topic, buffer);
}

void MQTTManager::loop() {
    _client->loop();
}
