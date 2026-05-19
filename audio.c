#include <stdio.h>
#include <stdlib.h>
#include <time.h>

///* void play() {
//
//	srand(time(NULL));

//	int rand_num = rand();
	


//}

#include "audio.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int pir_audio_init(PirAudioModule *module, int pin, const char *sound_file_path) {
    module->pin_number = pin;
    module->audio_path = sound_file_path;
    module->previous_value = 0;

    // Open GPIO chip (Pi 4 uses gpiochip4 usually, fallback to gpiochip0)
    module->chip = gpiod_chip_open_by_name("gpiochip4");
    if (!module->chip) {
        module->chip = gpiod_chip_open_by_name("gpiochip0");
        if (!module->chip) {
            perror("PIR Init: Failed to open GPIO chip");
            return -1;
        }
    }

    // Get the specified GPIO pin line
    module->line = gpiod_chip_get_line(module->chip, module->pin_number);
    if (!module->line) {
        perror("PIR Init: Failed to get GPIO line");
        gpiod_chip_close(module->chip);
        return -1;
    }

    // Configure the line as input
    if (gpiod_line_request_input(module->line, "PIR_Module") < 0) {
        perror("PIR Init: Failed to request input line");
        gpiod_chip_close(module->chip);
        return -1;
    }

    printf("PIR Audio Module successfully initialized on GPIO %d.\n", pin);
    return 0;
}

void pir_audio_update(PirAudioModule *module) {
    int current_value = gpiod_line_get_value(module->line);
    if (current_value < 0) return; // Error reading pin, skip this tick

    // Detect a rising edge (transition from LOW to HIGH)
    if (current_value == 1 && module->previous_value == 0) {
        printf("[PIR EVENT] Motion detected! Playing: %s\n", module->audio_path);
        
        char command[512];
        // Using mpg123 with standard background execution '&'
        snprintf(command, sizeof(command), "mpg123 -q %s &", module->audio_path);
        system(command);
        
        // Cooldown: a short sleep here stops sensor bounce.
        // If 2 seconds is too long for your servos, we can switch to a time-based check later.
        sleep(2); 
        
        // Force the check state update after sleeping
        current_value = gpiod_line_get_value(module->line);
    }

    module->previous_value = current_value;
}

void pir_audio_cleanup(PirAudioModule *module) {
    if (module->line) {
        gpiod_line_release(module->line);
    }
    if (module->chip) {
        gpiod_chip_close(module->chip);
    }
    printf("PIR Audio Module cleaned up.\n");
}
