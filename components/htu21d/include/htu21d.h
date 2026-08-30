#pragma once

#include "esp_err.h"
#include "driver/i2c_master.h"

esp_err_t htu21d_init(i2c_master_bus_handle_t bus);
esp_err_t htu21d_read(float *temperature, float *humidity);