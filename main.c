#include <stdio.h>
#include <unistd.h>
#include "audio.h"

// Placeholder inclusions for your future hardware modules
// #include "servo_control.h"
// #include "led_control.h"

int main(void) {
    PirAudioModule pir;
    
    // Initialize our module (GPIO 17, audio path)
    if (pir_audio_init(&pir, 17, "/home/pi/music/alert.mp3") != 0) { //change audio path
        return 1; // Exit if initialization failed
    }

    // Initialize your other components here later:
    // servo_init();
    // led_init();

    printf("Main robot loop starting...\n");

    // Super-loop executing all project actions concurrently
    while (1) {
        // 1. Check the PIR sensor & play audio if triggered
        pir_audio_update(&pir);

        // 2. Future Servo Logic (Non-blocking)
        // servo_update();

        // 3. Future LED Logic
        // led_update();

        // Small pause to prevent running your CPU at 100% load
        usleep(50000); // 50ms loop cycle
    }

    // Safety cleanup (though while(1) runs infinitely unless interrupted by Ctrl+C)
    pir_audio_cleanup(&pir);
    return 0;
}
