#pragma once

#include <gpiod.h>

typedef struct {
    struct gpiod_chip *chip;
    struct gpiod_line_request *line;
    int pin_number = 0;
    int previous_value = 0;
} PirSensor;

int pir_init(PirSensor *sensor, int pin);
int pir_check_motion(PirSensor *sensor);
void pir_cleanup(PirSensor *sensor);