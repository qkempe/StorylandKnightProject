#pragma once

// Adjust GPIO pin numbers to match your breadboard wiring
#define RED_PIN   18
#define GREEN_PIN 23
#define BLUE_PIN  24

void led_init();
void led_set_color(int red, int green, int blue);
void led_off();