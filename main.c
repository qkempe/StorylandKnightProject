#include <stdio.h>
#include <unistd.h>
#include "audio.h"

// Future modules:
// #include "servo.h"
// #include "led.h"

int main(void) {
    PirAudioModule pir;
    
    // We pass the FIRST filename here. 
    // We will handle the second one inside the update function 
    // or modify the path string to include both.
    
    // Let's assume GPIO 17 for the PIR sensor
    if (pir_audio_init(&pir, 17, "freesound_community-knight-spawn-97118.mp3") != 0) {
        fprintf(stderr, "Failed to initialize PIR sensor.\n");
        return 1;
    }

    printf("Robot System Online. Monitoring for motion...\n");

    while (1) {
        // 1. Check PIR and handle the audio sequence
        pir_audio_update(&pir);

        // 2. Placeholder for your future Servo/LED functions
        // update_servos();
        // update_leds();

        // Small delay (50ms) to prevent high CPU usage
        usleep(50000);
    }

    pir_audio_cleanup(&pir);
    return 0;
}
