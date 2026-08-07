#include "led.h"
#include <wiringPi.h>
#include <softPwm.h>
#include <stdio.h>

/*
void led_init() {
    softPwmCreate(RED_PIN, 0, 100);
    softPwmCreate(GREEN_PIN, 0, 100);
    softPwmCreate(BLUE_PIN, 0, 100);
}

void led_set_color(int red, int green, int blue) {
    softPwmWrite(RED_PIN, red);
    softPwmWrite(GREEN_PIN, green);
    softPwmWrite(BLUE_PIN, blue);
}

void led_off() {
    led_set_color(0, 0, 0);
}
*/

void led_init() {
	pinMode(LedPin, OUTPUT);    
}

void led_on() {
    digitalWrite(LedPin, LOW);
}

void led_off() {
    digitalWrite(LedPin, HIGH);

}

