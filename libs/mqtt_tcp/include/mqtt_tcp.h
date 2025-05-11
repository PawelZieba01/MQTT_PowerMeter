#ifndef MQTT_TCP_H
#define MQTT_TCP_H

#include "mqtt_client.h"

esp_mqtt_client_handle_t mqtt_init();
void mqtt_deinit(esp_mqtt_client_handle_t client);
void mqtt_publish(esp_mqtt_client_handle_t client, const char *topic, const char *data);

#endif // MQTT_TCP_H