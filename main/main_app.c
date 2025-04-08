#include <string.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_ota_ops.h"
#include "esp_app_format.h"
#include "esp_http_client.h"
#include "esp_flash_partitions.h"
#include "esp_partition.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
#include "protocol_examples_common.h"
#include "errno.h"
#include "esp_wifi.h"
#include "ota.h"


static const char *TAG = "main_app";


void app_main(void)
{
    ESP_LOGI(TAG, "Starting Main App...");

#ifndef CONFIG_OTA_UPDATE_SKIP
    xTaskCreate(&ota_task, "ota_task", 8192, NULL, 5, NULL);
#endif
}
