#pragma once

#include <gpiod.h>

// Structure to hold the state of our PIR hardware module
typedef struct {
    struct gpiod_chip *chip;
    struct gpiod_line_request *line;
    int pin_number;
    int previous_value;
    const char *audio_path;
} PirAudioModule;

// Initializes the GPIO chip and pin for the PIR sensor
// Returns 0 on success, -1 on failure
int pir_audio_init(PirAudioModule *module, int pin, const char *sound_file_path);

// Non-blocking check function to be called inside your main loop
void pir_audio_update(PirAudioModule *module);

// Cleans up resources when the program exits
void pir_audio_cleanup(PirAudioModule *module);

