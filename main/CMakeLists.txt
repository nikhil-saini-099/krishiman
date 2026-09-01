idf_component_register(
    SRCS "main.c"
    INCLUDE_DIRS "."
    REQUIRES soilsensor mcu_config I2C HTU21D adc
)