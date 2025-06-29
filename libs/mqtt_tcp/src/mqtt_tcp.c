#include "mqtt_tcp.h"
#include "esp_log.h"
#include "esp_err.h"

static const char *TAG = "MQTT";

static bool mqtt_connected = false;

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    esp_mqtt_event_handle_t event = event_data;

    switch (event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
            mqtt_connected = true;
            // Przykład subskrypcji tematu po połączeniu
            //esp_mqtt_client_subscribe(client, CONFIG_MQTT_SUB_TOPIC, 0);
            break;

        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
            mqtt_connected = false;
            break;

        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
            break;

        case MQTT_EVENT_UNSUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
            break;

        case MQTT_EVENT_PUBLISHED:
            ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
            break;

        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG, "MQTT_EVENT_DATA");
            ESP_LOGI(TAG, "TOPIC=%.*s", event->topic_len, event->topic);
            ESP_LOGI(TAG, "DATA=%.*s", event->data_len, event->data);
            break;

        case MQTT_EVENT_ERROR:
            ESP_LOGE(TAG, "MQTT_EVENT_ERROR");
            break;

        default:
            ESP_LOGI(TAG, "Other event id: %ld", event_id);
            break;
    }
}

esp_mqtt_client_handle_t mqtt_init()
{
    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = CONFIG_MQTT_BROKER_URL,
        .credentials.username = CONFIG_MQTT_BROKER_USERNAME,
        .credentials.authentication.password = CONFIG_MQTT_BROKER_PASSWORD,
        .broker.address.port = CONFIG_MQTT_BROKER_PORT,
    };

    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
    if (client == NULL) {
        ESP_LOGE(TAG, "Failed to initialize MQTT client");
        return NULL;
    }

    ESP_ERROR_CHECK(esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL));
    //ESP_ERROR_CHECK(esp_mqtt_client_start(client));
    ESP_LOGI(TAG, "MQTT client initialized");

    return client;
}

void mqtt_deinit(esp_mqtt_client_handle_t client)
{
    if (client) {
        ESP_ERROR_CHECK(esp_mqtt_client_stop(client));
        ESP_ERROR_CHECK(esp_mqtt_client_destroy(client));
        client = NULL;
        ESP_LOGI(TAG, "MQTT client stopped");
    }
}

void mqtt_publish(esp_mqtt_client_handle_t client, const char *topic, const char *data)
{
    if (client) {
        ESP_LOGI(TAG, "Publishing to topic: %s", topic);
        esp_mqtt_client_publish(client, topic, data, 0, 1, 0);
    } else {
        ESP_LOGE(TAG, "MQTT client is not initialized");
    }
}

void mqtt_disconnect(esp_mqtt_client_handle_t client)
{
    if (client) {
        esp_err_t err = esp_mqtt_client_stop(client);
        if (err == ESP_OK) {
            ESP_LOGI(TAG, "MQTT client disconnected successfully.");
        } else {
            ESP_LOGE(TAG, "Failed to disconnect MQTT client: %s", esp_err_to_name(err));
        }
    } else {
        ESP_LOGE(TAG, "MQTT client is not initialized.");
    }
}

void mqtt_connect(esp_mqtt_client_handle_t client)
{
    if (client) {
        esp_err_t err = esp_mqtt_client_start(client);
        if (err == ESP_OK) {
            ESP_LOGI(TAG, "MQTT client reconnected successfully.");
        } else {
            ESP_LOGE(TAG, "Failed to reconnect MQTT client: %s", esp_err_to_name(err));
        }
    } else {
        ESP_LOGE(TAG, "MQTT client is not initialized.");
    }
}

bool mqtt_is_connected(esp_mqtt_client_handle_t client)
{
    if (client) {
        if (mqtt_connected) {
            ESP_LOGI(TAG, "MQTT client is connected to the broker.");
            return true;
        } else {
            ESP_LOGW(TAG, "MQTT client is not connected to the broker.");
            return false;
        }
    } else {
        ESP_LOGE(TAG, "MQTT client is not initialized.");
        return false;
    }
}