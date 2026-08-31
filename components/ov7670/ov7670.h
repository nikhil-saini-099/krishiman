#ifndef OV7670_H
#define OV7670_H

#include <stdint.h>
#include "esp_err.h"

// SCCB (I2C) Address[cite: 9]
#define OV7670_SCCB_ADDR 0x21

// Hardware Pins (Adjust to your ESP32-S3 custom PCB/wiring)
#define CAM_PIN_SIOD    11 // I2C SDA
#define CAM_PIN_SIOC    12 // I2C SCL
#define CAM_PIN_VSYNC   13
#define CAM_PIN_HREF    14
#define CAM_PIN_PCLK    15
#define CAM_PIN_XCLK    10 // Requires 10-24MHz clock[cite: 9]
#define CAM_PIN_D7      9
#define CAM_PIN_D6      8
#define CAM_PIN_D5      7
#define CAM_PIN_D4      6
#define CAM_PIN_D3      5
#define CAM_PIN_D2      4
#define CAM_PIN_D1      3
#define CAM_PIN_D0      2

// Resolution
#define IMG_WIDTH  320
#define IMG_HEIGHT 240

// Functions
esp_err_t ov7670_init(void);
void ov7670_capture_frame(uint8_t *frame_buffer);

#endif // OV7670_H