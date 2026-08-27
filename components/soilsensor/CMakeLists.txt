idf_component_register(
    SRCS "soilsensor.c"
    INCLUDE_DIRS "include"
    REQUIRES adc mcu_config
)