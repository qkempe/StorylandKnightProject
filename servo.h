#pragma once

#define SERVO1_PIN 25
#define SERVO2_PIN 22

#define SERVO_CLOSED 5
#define SERVO_OPEN 15

void init_servos();
void set_servos_open();
void set_servos_closed();
void relax_servos();
