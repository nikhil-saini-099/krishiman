#include "ov7670.h"
#include "driver/i2c.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "rom/ets_sys.h" // For microsecond delays

static const char *TAG = "OV7670_DRIVER";

// I2C Configuration
#define I2C_MASTER_NUM      I2C_NUM_0
#define I2C_MASTER_FREQ_HZ  100000 

typedef struct {
    uint8_t reg;
    uint8_t val;
} ov7670_reg_t;

// Minimal register sequence for RGB565[cite: 9]
static const ov7670_reg_t cam_init_sequence[] = {
    {0x12, 0x80}, // COM7: Reset all registers[cite: 9]
    {0x12, 0x04}, // COM7: RGB selection[cite: 9]
    {0x40, 0xD0}, // COM15: RGB 565, Output range [00] to [FF][cite: 9]
    {0x8C, 0x00}, // RGB444: Disable[cite: 9]
    {0x11, 0x00}, // CLKRC: Use external clock[cite: 9]
    {0xFF, 0xFF}
};

static esp_err_t sccb_write_reg(uint8_t reg, uint8_t data) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (OV7670_SCCB_ADDR << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, reg, true);
    i2c_master_write_byte(cmd, data, true);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, pdMS_TO_TICKS(1000));
    i2c_cmd_link_delete(cmd);
    return ret;
}

static void init_dvp_gpios(void) {
    gpio_config_t io_conf = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_INPUT,
        .pin_bit_mask = (1ULL<<CAM_PIN_VSYNC) | (1ULL<<CAM_PIN_HREF) | (1ULL<<CAM_PIN_PCLK) |
                        (1ULL<<CAM_PIN_D7) | (1ULL<<CAM_PIN_D6) | (1ULL<<CAM_PIN_D5) |
                        (1ULL<<CAM_PIN_D4) | (1ULL<<CAM_PIN_D3) | (1ULL<<CAM_PIN_D2) |
                        (1ULL<<CAM_PIN_D1) | (1ULL<<CAM_PIN_D0),
        .pull_down_en = 0,
        .pull_up_en = 0
    };
    gpio_config(&io_conf);
}

esp_err_t ov7670_init(void) {
    // 1. Setup I2C
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = CAM_PIN_SIOD,
        .scl_io_num = CAM_PIN_SIOC,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };
    i2c_param_config(I2C_MASTER_NUM, &conf);
    i2c_driver_install(I2C_MASTER_NUM, conf.mode, 0, 0, 0);

    // 2. Setup DVP Pins
    init_dvp_gpios();

    // 3. Write Initialization Registers
    for (int i = 0; cam_init_sequence[i].reg != 0xFF; i++) {
        if (sccb_write_reg(cam_init_sequence[i].reg, cam_init_sequence[i].val) != ESP_OK) {
            ESP_LOGE(TAG, "Failed to write register 0x%02x", cam_init_sequence[i].reg);
            return ESP_FAIL;
        }
        if (cam_init_sequence[i].reg == 0x12 && cam_init_sequence[i].val == 0x80) {
            vTaskDelay(pdMS_TO_TICKS(10)); // Delay after reset[cite: 9]
        }
    }
    
    ESP_LOGI(TAG, "OV7670 initialized successfully");
    return ESP_OK;
}

// Inline for max speed during tight PCLK polling
static inline uint8_t read_data_pins(void) {
    // Note: If D0-D7 are contiguous on the ESP32-S3 GPIO port, 
    // this can be optimized to a single direct register read.
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

    // Wait for VSYNC to signal start of frame[cite: 9]
    while (gpio_get_level(CAM_PIN_VSYNC) == 0); 
    while (gpio_get_level(CAM_PIN_VSYNC) == 1);

    for (int row = 0; row < IMG_HEIGHT; row++) {
        // Wait for valid row[cite: 9]
        while (gpio_get_level(CAM_PIN_HREF) == 0);

        for (int col = 0; col < IMG_WIDTH * 2; col++) {
            // Wait for PCLK rising edge[cite: 9]
            while (gpio_get_level(CAM_PIN_PCLK) == 0);
            
            frame_buffer[index++] = read_data_pins();
            
            // Wait for PCLK falling edge[cite: 9]
            while (gpio_get_level(CAM_PIN_PCLK) == 1);
        }
    }
}