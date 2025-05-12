#ifndef LIGHT_SENOR_H
#define LIGHT_SENSOR_H

#include <stdint.h>

typedef struct {
    uint16_t gpio_pin;
    uint16_t pulse_polarity;
    volatile uint16_t pulse_counter;
} light_sensor_t;

light_sensor_t * light_sensor_init();
uint16_t light_sensor_get_count(light_sensor_t *light_sensor);
void light_sensor_reset_count(light_sensor_t *light_sensor);

#endif // LIGHT_SENSOR_H