#include <stdio.h>
#include <stdlib.h>
#include <time.h>

///* void play() {
//
//	srand(time(NULL));

//	int rand_num = rand();
	


#include "audio.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <gpiod.h>

int pir_audio_init(PirAudioModule *module, int pin, const char *sound_file_path) {
    module->pin_number = pin;
    module->audio_path = sound_file_path;
    module->previous_value = 0;

    // In v2.x, we open the chip path directly
    module->chip = gpiod_chip_open("/dev/gpiochip4");
    if (!module->chip) {
        module->chip = gpiod_chip_open("/dev/gpiochip0");
        if (!module->chip) {
            perror("PIR Init: Failed to open GPIO chip");
            return -1;
        }
    }

    // Prepare line settings: Set direction to INPUT
    struct gpiod_line_settings *settings = gpiod_line_settings_new();
    gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_INPUT);

    // Create a config for the specific line (pin)
    struct gpiod_line_config *line_cfg = gpiod_line_config_new();
    gpiod_line_config_add_line_settings(line_cfg, &module->pin_number, 1, settings);

    // Request the line
    struct gpiod_request_config *req_cfg = gpiod_request_config_new();
    gpiod_request_config_set_consumer(req_cfg, "PIR_Module");

    module->line = gpiod_chip_request_lines(module->chip, req_cfg, line_cfg);

    // Clean up temporary config objects
    gpiod_line_settings_free(settings);
    gpiod_line_config_free(line_cfg);
    gpiod_request_config_free(req_cfg);

    if (!module->line) {
        perror("PIR Init: Failed to request lines");
        gpiod_chip_close(module->chip);
        return -1;
    }

    printf("PIR Audio Module (v2 API) initialized on GPIO %d.\n", pin);
    return 0;
}

void pir_audio_update(PirAudioModule *module) {
    // In v2, we use gpiod_line_request_get_value
    enum gpiod_line_value val = gpiod_line_request_get_value(module->line, module->pin_number);
    
    int current_value = (val == GPIOD_LINE_VALUE_ACTIVE) ? 1 : 0;

    if (current_value == 1 && module->previous_value == 0) {
        printf("[PIR EVENT] Motion detected!\n");
        
        char command[512];
        snprintf(command, sizeof(command), "mpg123 -q %s &", module->audio_path);
        system(command);
        
        sleep(2); 
        // Re-sync state after sleep
        val = gpiod_line_request_get_value(module->line, module->pin_number);
        current_value = (val == GPIOD_LINE_VALUE_ACTIVE) ? 1 : 0;
    }

    module->previous_value = current_value;
}

void pir_audio_cleanup(PirAudioModule *module) {
    if (module->line) {
        gpiod_line_request_release(module->line);
    }
    if (module->chip) {
        gpiod_chip_close(module->chip);
    }
    printf("PIR Audio Module cleaned up.\n");
}
