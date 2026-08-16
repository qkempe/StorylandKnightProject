#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>

#include "servo.h"
#include "pir.h"
#include "audio.h"
#include "led.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define COOLDOWN_MS 5000 // 5 second reset window between visitors

int main() {
    PirSensor pir;
    srand(time(NULL)); //start random seed

    if (pir_init(&pir, 17) != 0) {
        fprintf(stderr, "Failed to initialize PIR sensor.\n");
        return 1;
    }

    if (wiringPiSetup() == -1) {
	    printf("setup wiringPi failed");
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
            
            led_on(); // set eyes to red
            
            // Play one of the eight audios
	    int num = rand() % 8;
	    if (num == 0) {
            	play_audio_async("/home/diego/StorylandKnightProject/freesound_community-knight-spawn-97118.mp3");
            	play_audio_async("/home/diego/StorylandKnightProject/wings_of_freedom-draw-sword-490796.mp3");
	    } else if (num == 1) {
		play_audio_async("/home/diego/StorylandKnightProject/phatphrogstudio-rpg-m-wizard-voice-greeting-player-no-ai-479595.mp3");
            	play_audio_async("/home/diego/StorylandKnightProject/wings_of_freedom-draw-sword-490796.mp3");
	    } else if (num == 2) {
	  	play_audio_async("/home/diego/StorylandKnightProject/phatphrogstudio-rpg-m-knight-voice-stand-ready-477946.mp3");
            	play_audio_async("/home/diego/StorylandKnightProject/wings_of_freedom-draw-sword-490796.mp3");
	    } else if (num == 3) {
	  	play_audio_async("/home/diego/StorylandKnightProject/phatphrogstudio-rpg-m-knight-voice-attack-grunt-490291.mp3");
            	play_audio_async("/home/diego/StorylandKnightProject/wings_of_freedom-draw-sword-490796.mp3");
	    } else if (num == 4) {
	  	play_audio_async("/home/diego/StorylandKnightProject/originalvo-medieval-gamer-voice-wisdom-will-come-my-friend-226577.mp3");
            	play_audio_async("/home/diego/StorylandKnightProject/wings_of_freedom-draw-sword-490796.mp3");
	    } else if (num == 5) {
	  	play_audio_async("/home/diego/StorylandKnightProject/originalvo-medieval-gamer-voice-to-battle-226575.mp3");
            	play_audio_async("/home/diego/StorylandKnightProject/wings_of_freedom-draw-sword-490796.mp3");
	    } else if (num == 6) {
	  	play_audio_async("/home/diego/StorylandKnightProject/originalvo-medieval-gamer-voice-the-day-is-ours-226579.mp3");
            	play_audio_async("/home/diego/StorylandKnightProject/wings_of_freedom-draw-sword-490796.mp3");
	    } else { // == 7
	  	play_audio_async("/home/diego/StorylandKnightProject/originalvo-medieval-gamer-voice-darkness-hunts-us-what-youx27ve-learned-stay-226596.mp3");
            	play_audio_async("/home/diego/StorylandKnightProject/wings_of_freedom-draw-sword-490796.mp3");
	    }

            // Perform movement
            set_servos_open();
            delay(3000);

            set_servos_closed();
            delay(500);

            // Relax motors to prevent overheating/jitter
            relax_servos();
            led_off(); //turn off eyes

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
