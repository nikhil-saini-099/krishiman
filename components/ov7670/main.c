#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "ov7670.h"

static const char *TAG = "KIRSHI_MAIN";

// Allocate memory for the image (Width * Height * 2 bytes for RGB565)[cite: 9]
// Use DRAM allocation or static buffer.
static uint8_t image_buffer[IMG_WIDTH * IMG_HEIGHT * 2];

void app_main(void) {
    ESP_LOGI(TAG, "Starting Krishiman Camera Node");

    // Initialize OV7670
    if (ov7670_init() != ESP_OK) {
        ESP_LOGE(TAG, "Camera initialization failed. Halting.");
        return;
    }

    while (1) {
        ESP_LOGI(TAG, "Capturing frame...");
        
        // This blocks until a full frame is captured
        ov7670_capture_frame(image_buffer);
        
        ESP_LOGI(TAG, "Frame captured! Passing to AI model.");

        // TODO: Pass 'image_buffer' to TensorFlow Lite Micro or cloud endpoint here
        // run_ai_inference(image_buffer);

        vTaskDelay(pdMS_TO_TICKS(5000)); // Delay between captures
    }
}