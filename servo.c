#include "servo.h"
#include <wiringPi.h>
#include <softPwm.h>

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
    // writing 0 stops software pulse to eliminate jitter
    softPwmWrite(SERVO1_PIN, 0);
    softPwmWrite(SERVO2_PIN, 0);
}