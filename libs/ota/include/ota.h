#ifndef OTA_H
#define OTA_H

#include "esp_err.h"

esp_err_t ota_update(void);
void ota_init(void);

#endif // OTA_H