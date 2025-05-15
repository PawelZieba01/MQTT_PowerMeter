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
        while (wifi_is_connected() == false) {
            ESP_LOGI(TAG, "Waiting for WiFi connection...");
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
        uint8_t buffer[8];
        sprintf((char *)buffer, "%d", pulse_count);
        ESP_LOGI(TAG, "Captured impulses number = %d", pulse_count);
        mqtt_publish(mqtt_client, "example/topic", (const char *)buffer);

        // Disconnect MQTT and stop WiFi
        ESP_LOGI(TAG, "Disconnecting MQTT in Loop");
        mqtt_disconnect(mqtt_client);
        ESP_LOGI(TAG, "Stopping WiFi in Loop");
        wifi_stop();

        ESP_LOGI(TAG, "Main App loop completed. Communitacion peripheral is deinitialized. Waiting for next iteration...");
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(CONFIG_MQTT_PUBLISH_INTERVAL * 1000));
    }
}
