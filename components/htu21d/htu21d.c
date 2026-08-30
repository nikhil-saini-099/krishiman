#include "htu21d.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define HTU21D_ADDR          0x40
#define HTU21D_TEMP_CMD      0xF3
#define HTU21D_HUMIDITY_CMD  0xF5

static i2c_master_dev_handle_t htu21d_dev;
// Initialize the HTU21D on the i2c bus
esp_err_t htu21d_init(i2c_master_bus_handle_t bus)
{
    i2c_device_config_t dev_config = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = HTU21D_ADDR,
        .scl_speed_hz = 100000,
    };

    return i2c_master_bus_add_device( bus, &dev_config,&htu21d_dev );
}
// send measurement command and read raw sensor data
static esp_err_t read_raw(uint8_t command, uint16_t *raw)
{
    uint8_t data[3];

    esp_err_t ret = i2c_master_transmit( htu21d_dev, &command,1,1000);

    if (ret != ESP_OK) {
        return ret;
    }

    vTaskDelay(pdMS_TO_TICKS(50));

    ret = i2c_master_receive(htu21d_dev,data, 3,1000);

    if (ret != ESP_OK) {
        return ret;
    }

    *raw = ((uint16_t)data[0] << 8) | data[1];

    *raw &= 0xFFFC;

    return ESP_OK;
}
// convert raw sensor data to temperature and humidity values
esp_err_t htu21d_read(float *temperature,float *humidity)
{
    uint16_t raw_temperature;
    uint16_t raw_humidity;

    esp_err_t ret = read_raw( HTU21D_TEMP_CMD,&raw_temperature);

    if (ret != ESP_OK) {
        return ret;
    }

    ret = read_raw( HTU21D_HUMIDITY_CMD, &raw_humidity);

    if (ret != ESP_OK) {
        return ret;
    }

    *temperature = -46.85f + (175.72f * raw_temperature / 65536.0f);

    *humidity = -6.0f + (125.0f * raw_humidity / 65536.0f);

    if (*humidity < 0.0f) {
        *humidity = 0.0f;
    }

    if (*humidity > 100.0f) {
        *humidity = 100.0f;
    }

    return ESP_OK;
}