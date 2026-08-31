idf_component_register(
    SRCS "main.c" "ov7670.c"
    INCLUDE_DIRS "."
    REQUIRES driver esp_driver_gpio esp_driver_i2c
)