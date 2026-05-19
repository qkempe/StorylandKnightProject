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

void play_sound_sequence(const char *file1, const char *file2) {
    char command1[512];
    char command2[512];

    // NO '&' here: This makes the program wait for file1 to finish
    printf("Playing first sound...\n");
    snprintf(command1, sizeof(command1), "mpg123 -q %s", file1);
    system(command1);

    // This line won't run until the line above is totally done
    printf("Playing second sound...\n");
    snprintf(command2, sizeof(command2), "mpg123 -q %s &", file2);
    system(command2);
}
void pir_audio_update(PirAudioModule *module) {
    enum gpiod_line_value val = gpiod_line_request_get_value(module->line, module->pin_number);
    int current_value = (val == GPIOD_LINE_VALUE_ACTIVE) ? 1 : 0;

    if (current_value == 1 && module->previous_value == 0) {
        printf("[EVENT] Motion detected! Playing audio sequence...\n");

        char command[1024];
        // This command says: 
        // Play freesound, AND THEN (&&) play wings_of_freedom. 
        // The '&' at the very end puts the WHOLE sequence in the background.
        /*snprintf(command, sizeof(command), 
                 "mpg123 -q %s && mpg123 -q wings_of_freedom-draw-sword-490796.mp3 &", 
                 module->audio_path);*/
        // Inside pir_audio_update
		snprintf(command, sizeof(command), 
         "mpg123 -q ./%s && mpg123 -q ./wings_of_freedom-draw-sword-490796.mp3 &", 
         module->audio_path);
        system(command);

        // A 6-second cooldown so it doesn't restart the sequence 
        // while the audio is still playing.
        sleep(3); 
    }
    module->previous_value = current_value;
}
/*void pir_audio_update(PirAudioModule *module) {
    enum gpiod_line_value val = gpiod_line_request_get_value(module->line, module->pin_number);
    int current_value = (val == GPIOD_LINE_VALUE_ACTIVE) ? 1 : 0;

    if (current_value == 1 && module->previous_value == 0) {
        printf("[PIR EVENT] Starting sequence...\n");

        // 1. Play the "Intro" or "Alert" sound (Waits for it to finish)
        system("mpg123 -q ./alert_signal.mp3");

        // 2. Play the "Voice" or "Effect" sound (Moves on immediately)
        system("mpg123 -q ./voice_message.mp3 &");

        // Cooldown
        sleep(2);
    }
    module->previous_value = current_value;
}*/
/*
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
} */

void pir_audio_cleanup(PirAudioModule *module) {
    if (module->line) {
        gpiod_line_request_release(module->line);
    }
    if (module->chip) {
        gpiod_chip_close(module->chip);
    }
    printf("PIR Audio Module cleaned up.\n");
}
