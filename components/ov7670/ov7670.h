#ifndef OV7670_H
#define OV7670_H

#include <stdint.h>
#include "esp_err.h"
#include "driver/i2c_master.h"

#define OV7670_SCCB_ADDR 0x21

#define CAM_PIN_VSYNC   13
#define CAM_PIN_HREF    14
#define CAM_PIN_PCLK    15
#define CAM_PIN_XCLK    10 
#define CAM_PIN_D7      9
#define CAM_PIN_D6      8
#define CAM_PIN_D5      7
#define CAM_PIN_D4      6
#define CAM_PIN_D3      1  
#define CAM_PIN_D2      4
#define CAM_PIN_D1      3
#define CAM_PIN_D0      2

#define IMG_WIDTH  320
#define IMG_HEIGHT 240

esp_err_t ov7670_init(i2c_master_bus_handle_t bus_handle);
void ov7670_capture_frame(uint8_t *frame_buffer);

#endif