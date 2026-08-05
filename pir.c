#include "pir.h"
#include <stdio.h>
#include <stdlib.h>

int pir_init(PirSensor *sensor, int pin) {
    sensor->pin_number = pin;
    sensor->previous_value = 0;

    sensor->chip = gpiod_chip_open("/dev/gpiochip4");
    if (!sensor->chip) {
        sensor->chip = gpiod_chip_open("/dev/gpiochip0");
        if (!sensor->chip) {
            perror("PIR Init: Failed to open GPIO chip");
            return -1;
        }
    }

    struct gpiod_line_settings *settings = gpiod_line_settings_new();
    gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_INPUT);

    struct gpiod_line_config *line_cfg = gpiod_line_config_new();
    gpiod_line_config_add_line_settings(line_cfg, &sensor->pin_number, 1, settings);

    struct gpiod_request_config *req_cfg = gpiod_request_config_new();
    gpiod_request_config_set_consumer(req_cfg, "PIR_Sensor");

    sensor->line = gpiod_chip_request_lines(sensor->chip, req_cfg, line_cfg);

    gpiod_line_settings_free(settings);
    gpiod_line_config_free(line_cfg);
    gpiod_request_config_free(req_cfg);

    if (!sensor->line) {
        perror("PIR Init: Failed to request lines");
        gpiod_chip_close(sensor->chip);
        return -1;
    }

    printf("PIR Sensor initialized on GPIO %d.\n", pin);
    return 0;
}

int pir_check_motion(PirSensor *sensor) {
    enum gpiod_line_value val = gpiod_line_request_get_value(sensor->line, sensor->pin_number);
    int current_value = (val == GPIOD_LINE_VALUE_ACTIVE);
    int triggered = 0;

    if (current_value == 1 && sensor->previous_value == 0) {
        triggered = 1;
    }

    sensor->previous_value = current_value;
    return triggered;
}

void pir_cleanup(PirSensor *sensor) {
    if (sensor->line) {
        gpiod_line_request_release(sensor->line);
    }
    if (sensor->chip) {
        gpiod_chip_close(sensor->chip);
    }
    printf("PIR Sensor cleaned up.\n");
}