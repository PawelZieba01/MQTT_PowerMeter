#include <string.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "errno.h"

#include "ota.h"


static const char *TAG = "main_app";


void app_main(void)
{
    ESP_LOGI(TAG, "Starting Main App...");

#ifndef CONFIG_OTA_UPDATE_SKIP
    ota_init();
    if (ota_update() != ESP_OK) {
        ESP_LOGW(TAG, "OTA update failed");
        ESP_LOGW(TAG, "Firmware update ignored, running the current version. Probably server is not running.");
    }
#endif

    ESP_LOGI(TAG, "Main App started successfully.");
    // Add your main application code here.
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
        ESP_LOGI(TAG, "Main App is running...");
    }
}
