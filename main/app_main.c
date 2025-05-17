#include <string.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_err.h"
#include "errno.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "nvs.h"
#include "nvs_flash.h"

#include "ota.h"
#include "wifi.h"
#include "mqtt_tcp.h"
#include "light_sensor.h"



static const char *TAG = "main_app";

static esp_mqtt_client_handle_t mqtt_client;
static light_sensor_t *light_sensor = NULL;



void app_main(void)
{
    ESP_LOGI(TAG, "Starting Main App...");

    // Initialize NVS.
    ESP_LOGI(TAG, "Initializing NVS");
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK( err );

    // Initialize WiFi.
    ESP_LOGI(TAG, "Initializing WiFi");
    wifi_init_sta();

    // Initialize OTA.
#ifndef CONFIG_OTA_UPDATE_SKIP
    ota_init();
    if (ota_update() != ESP_OK) {
        ESP_LOGW(TAG, "OTA update failed");
        ESP_LOGW(TAG, "Firmware update ignored, running the current version. Probably server is not running.");
    }
#endif
        
    // Initialize light sensor.
    light_sensor = light_sensor_init();
    if (light_sensor == NULL) {
        ESP_LOGE(TAG, "Failed to initialize light sensor");
    }

    // Initialize MQTT.
    ESP_LOGI(TAG, "Initializing MQTT in Loop");
    mqtt_client = mqtt_init();

    // Stop WiFi to save power.
    wifi_stop();
    
    ESP_LOGI(TAG, "Main App started successfully.");


    // Main loop
    TickType_t xLastWakeTime = xTaskGetTickCount();
    while (1) {

        // Start WiFi connection
        wifi_start();
        uint8_t wifi_retries = 0;
        while (wifi_is_connected() == false) {
            ESP_LOGI(TAG, "Waiting for WiFi connection...");
            wifi_retries++;
            if (wifi_retries > 15) {
                ESP_LOGE(TAG, "Failed to connect to WiFi after 10 attempts. Stopping WiFi.");
                esp_restart();
                break;
            }
            vTaskDelay(pdMS_TO_TICKS(1000));
        }

        //Start MQTT connection 
        ESP_LOGI(TAG, "Starting MQTT connection in Loop");
        mqtt_connect(mqtt_client);
        while(mqtt_is_connected(mqtt_client) == false) {
            ESP_LOGI(TAG, "Waiting for MQTT connection...");
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
                
        //Get the pulse count from the light sensor
        uint16_t pulse_count = light_sensor_get_count(light_sensor);
        
        // Reset the pulse counter
        light_sensor_reset_count(light_sensor);

        // Publish the pulse count to the MQTT topic
        uint8_t pulse_count_buf[8];
        sprintf((char *)pulse_count_buf, "%d", pulse_count);
        ESP_LOGI(TAG, "Captured impulses number = %d", pulse_count);

        // Get the IP address as a string
        char ip_str[16];
        if (wifi_get_ip_str(ip_str, sizeof(ip_str))) {
            ESP_LOGI(TAG, "IP as string: %s", ip_str);
        }

        // Prepare the MQTT topic
        char topic[64] = CONFIG_PROJECT_MQTT_PUB_TOPIC;
        strcat(topic, strrchr(ip_str, '.') + 1);
        ESP_LOGI(TAG, "MQTT Topic: %s", topic);

        // pUblish the pulse count to the MQTT topic
        mqtt_publish(mqtt_client, topic, (const char *)pulse_count_buf);

        // Disconnect MQTT and stop WiFi
        ESP_LOGI(TAG, "Disconnecting MQTT in Loop");
        mqtt_disconnect(mqtt_client);
        ESP_LOGI(TAG, "Stopping WiFi in Loop");
        wifi_stop();

        ESP_LOGI(TAG, "Main App loop completed. Communitacion peripheral is deinitialized. Waiting for next iteration...");
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(CONFIG_MQTT_PUBLISH_INTERVAL * 1000));
    }
}
