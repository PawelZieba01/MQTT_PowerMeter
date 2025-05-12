#include "light_sensor.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_err.h"
#include "esp_attr.h"


static const char *TAG = "light_sensor";


static void IRAM_ATTR gpio_isr_handler(void *arg) {
    light_sensor_t *light_sensor = (light_sensor_t *)arg;

    uint16_t level = gpio_get_level(light_sensor->gpio_pin);
    if (level == light_sensor->pulse_polarity) {
        light_sensor->pulse_counter++;
    }
}


light_sensor_t * light_sensor_init() {
    // Allocate memory for light sensor data
    light_sensor_t *light_sensor = malloc(sizeof(light_sensor_t));
    if (light_sensor == NULL) {
        ESP_LOGE(TAG, "Failed to allocate memory for light sensor data");
        return NULL;
    }

    light_sensor->pulse_polarity = CONFIG_SENSOR_PIN_POLARITY;
    light_sensor->gpio_pin = CONFIG_SENSOR_INPUT_PIN;
    light_sensor->pulse_counter = 0;

    // Configure GPIO
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << light_sensor->gpio_pin),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_ANYEDGE
    };
    gpio_config(&io_conf);

    // Install ISR service
    gpio_install_isr_service(0);
    gpio_isr_handler_add(light_sensor->gpio_pin, gpio_isr_handler, (void *)light_sensor);

    return light_sensor;
}


uint16_t light_sensor_get_count(light_sensor_t *light_sensor) {
    return light_sensor->pulse_counter;
}


void light_sensor_reset_count(light_sensor_t *light_sensor) {
    light_sensor->pulse_counter = 0;
}
