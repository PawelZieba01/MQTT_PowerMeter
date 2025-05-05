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


static const char *TAG = "main_app";


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

    ESP_LOGI(TAG, "Main App started successfully.");


    
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
        ESP_LOGI(TAG, "Main App is running...");
    }
}
