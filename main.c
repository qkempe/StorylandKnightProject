#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>

#include "servo.h"
#include "pir.h"
#include "audio.h"
#include "led.h"

#define COOLDOWN_MS 5000 // 5 second reset window between visitors

int main() {
    PirSensor pir;

    if (pir_init(&pir, 17) != 0) {
        fprintf(stderr, "Failed to initialize PIR sensor.\n");
        return 1;
    }

    init_servos();
    led_init();

    printf("Syncing servos to starting position...\n");
    set_servos_closed();
    delay(1000);
    relax_servos();

    printf("Storyland Exhibit Ready. Monitoring for visitors...\n");

    while (1) {
        if (pir_check_motion(&pir)) {
            printf("[EVENT] Visitor detected! Starting sequence...\n");
            
            led_set_color(100, 0, 0); // set eyes to red
            
            // Play audio
            play_audio_async("freesound_community-knight-spawn-97118.mp3");
            play_audio_async("wings_of_freedom-draw-sword-490796.mp3");

            // Perform movement
            set_servos_open();
            delay(3000);

            set_servos_closed();
            delay(500);

            // Relax motors to prevent overheating/jitter
            relax_servos();
            led_set_color(0, 0, 0); //turn off eyes

            // Wait out the PIR sensor high-time / give space before next trigger
            printf("Sequence finished. Entering cooldown period...\n");
            delay(COOLDOWN_MS);
            
            // Clear out any lingering motion state before returning to polling
            pir_check_motion(&pir);
            printf("Ready for next visitor.\n");
        }

        // Low-CPU sleep polling interval (~50ms)
        usleep(50000);
    }

    // Unreachable in kiosk mode unless interrupted (e.g. SIGINT)
    pir_cleanup(&pir);
    led_off();
    
    return 0;
}