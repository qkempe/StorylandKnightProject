#pragma once

// Adjust GPIO pin numbers to match your breadboard wiring
#define LEDPIN1 23
#define LEDPIN1 24

extern int status;

void led_init();
void led_on();
void led_off();
