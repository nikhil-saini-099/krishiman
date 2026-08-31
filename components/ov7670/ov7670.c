#include "ov7670.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "OV7670_DRIVER";
static i2c_master_dev_handle_t sccb_dev;

typedef struct {
    uint8_t reg;
    uint8_t val;
} ov7670_reg_t;

static const ov7670_reg_t cam_init_sequence[] = {
    {0x12, 0x80}, // Reset registers
    {0x12, 0x04}, // Output RGB
    {0x40, 0xD0}, // RGB 565 format
    {0x8C, 0x00}, // Disable RGB444
    {0x11, 0x00}, // External clock
    {0xFF, 0xFF}
};

static esp_err_t sccb_write_reg(uint8_t reg, uint8_t data) {
    uint8_t write_buf[2] = {reg, data};
    return i2c_master_transmit(sccb_dev, write_buf, 2, 1000);
}

esp_err_t ov7670_init(i2c_master_bus_handle_t bus_handle) {
    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = OV7670_SCCB_ADDR,
        .scl_speed_hz = 100000,
    };

    esp_err_t err = i2c_master_bus_add_device(bus_handle, &dev_cfg, &sccb_dev);
    if (err != ESP_OK) return err;

    // Configure GPIOs
    gpio_config_t io_conf = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_INPUT,
        .pin_bit_mask = (1ULL<<CAM_PIN_VSYNC) | (1ULL<<CAM_PIN_HREF) | (1ULL<<CAM_PIN_PCLK) |
                        (1ULL<<CAM_PIN_D7) | (1ULL<<CAM_PIN_D6) | (1ULL<<CAM_PIN_D5) |
                        (1ULL<<CAM_PIN_D4) | (1ULL<<CAM_PIN_D3) | (1ULL<<CAM_PIN_D2) |
                        (1ULL<<CAM_PIN_D1) | (1ULL<<CAM_PIN_D0),
    };
    gpio_config(&io_conf);

    for (int i = 0; cam_init_sequence[i].reg != 0xFF; i++) {
        if (sccb_write_reg(cam_init_sequence[i].reg, cam_init_sequence[i].val) != ESP_OK) {
            ESP_LOGE(TAG, "SCCB Write Failed at reg 0x%02x", cam_init_sequence[i].reg);
            return ESP_FAIL;
        }
        if (cam_init_sequence[i].reg == 0x12 && cam_init_sequence[i].val == 0x80) {
            vTaskDelay(pdMS_TO_TICKS(10));
        }
    }
    return ESP_OK;
}

static inline uint8_t read_data_pins(void) {
    return (gpio_get_level(CAM_PIN_D7) << 7) |
           (gpio_get_level(CAM_PIN_D6) << 6) |
           (gpio_get_level(CAM_PIN_D5) << 5) |
           (gpio_get_level(CAM_PIN_D4) << 4) |
           (gpio_get_level(CAM_PIN_D3) << 3) |
           (gpio_get_level(CAM_PIN_D2) << 2) |
           (gpio_get_level(CAM_PIN_D1) << 1) |
           (gpio_get_level(CAM_PIN_D0) << 0);
}

void ov7670_capture_frame(uint8_t *frame_buffer) {
    uint32_t index = 0;
    while (gpio_get_level(CAM_PIN_VSYNC) == 0); 
    while (gpio_get_level(CAM_PIN_VSYNC) == 1);

    for (int row = 0; row < IMG_HEIGHT; row++) {
        while (gpio_get_level(CAM_PIN_HREF) == 0);
        for (int col = 0; col < IMG_WIDTH * 2; col++) {
            while (gpio_get_level(CAM_PIN_PCLK) == 0);
            frame_buffer[index++] = read_data_pins();
            while (gpio_get_level(CAM_PIN_PCLK) == 1);
        }
    }
}