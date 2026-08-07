#pragma once

// Adjust GPIO pin numbers to match your breadboard wiring
#define LedPin 0
#define GREEN_PIN 23
#define BLUE_PIN  24

void led_init();
void led_on();
void led_off();
