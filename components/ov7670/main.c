#include "driver/i2c_master.h"
#include "htu21d.h"
#include "ov7670.h"
#include "adc.h"

void app_main(void) {
    // 1. Initialize Single Shared I2C Bus (SDA: GPIO 11, SCL: GPIO 12)
    i2c_master_bus_config_t i2c_bus_config = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = I2C_NUM_0,
        .scl_io_num = 12,
        .sda_io_num = 11,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };
    i2c_master_bus_handle_t bus_handle;
    ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_bus_config, &bus_handle));

    // 2. Initialize Peripherals on Shared Bus
    htu21d_init(bus_handle);[cite: 12, 15]
    ov7670_init(bus_handle);
    adc_init();[cite: 10, 17]
}