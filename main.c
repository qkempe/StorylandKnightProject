#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>
#include <softPwm.h>
#include "audio.h"

#define SERVO1_PIN 24
#define SERVO2_PIN 22

#define SERVO_CLOSED 5
#define SERVO_OPEN 15

void init_servos() {
    wiringPiSetupGpio(); 
    softPwmCreate(SERVO1_PIN, SERVO_CLOSED, 200);
    softPwmCreate(SERVO2_PIN, SERVO_CLOSED, 200);
}

void set_servos_open() {
    softPwmWrite(SERVO1_PIN, SERVO_OPEN);
    softPwmWrite(SERVO2_PIN, SERVO_OPEN);
}

void set_servos_closed() {
    softPwmWrite(SERVO1_PIN, SERVO_CLOSED);
    softPwmWrite(SERVO2_PIN, SERVO_CLOSED);
}

void relax_servos() {
    // writing a 0 stops the software pulse completely to stop jitter
    softPwmWrite(SERVO1_PIN, 0);
    softPwmWrite(SERVO2_PIN, 0);
}

int main(void) {
    PirAudioModule pir;

    if (pir_audio_init(&pir, 17, "freesound_community-knight-spawn-97118.mp3") != 0) {
        fprintf(stderr, "Failed to initialize PIR sensor.\n");
        return 1;
    }

    init_servos();
    
    printf("Syncing servos to starting position...\n");
    set_servos_closed();
    delay(1000);
    relax_servos();

    printf("Robot System Online. Monitoring for motion...\n");

    while (1) {
        if (pir_audio_update(&pir) == 1) {
            
            set_servos_open();
            
            // keep them open and block new motion for 3 seconds
            delay(3000);
            
            set_servos_closed();
            
            // give the motors 500 milliseconds to physically move back
            delay(500);
            
            // turn off the signal so they sit quietly while waiting for motion
            relax_servos();
        }

        usleep(50000);
    }

    pir_audio_cleanup(&pir);
    return 0;
}
