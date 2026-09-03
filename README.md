# Storyland Project

## Overview
An automated, motion-activated robotic exhibit designed and commissioned for *Storyland Fresno*. The system uses a PIR sensor to detect approaching visitors, animating the knight via servo motors while playing synchronized audio through a Bluetooth speaker. Built in C and executed as a headless user-level systemd daemon on a Raspberry Pi 4 that runs 24/7.
### Features
-PIR motion detection

-Servo motor control

-Red lights

-Background audio streaming via Pulsewire/mpg123


## Hardware
-Raspberry Pi 4

-PIR Motion Sensor

-Servo Motors

-RGB LEDS

-Bluetooth Speaker

## Prerequisites to install
sudo apt update

sudo apt install gcc mpg123 bluetooth bluez pipewire

## Systemd User Service Setup
1. create service in ~/.config/systemd/user/

2. enable lingering: sudo loginctl enable-linger $USER

3. run systemctl --user daemon-reload
  
4. run systemctl --user enable --now knight.service

## Service Command Management
**Made with aliases in .bashrc**

**Start:** systemctl --user start knight.service (Starts the executable in the current session)

**Enable:** systemctl --user enable knight.service (Tell system to run the service when pi boots up)

**Disable:** systemctl --user disable knight.service (Removes the service from the boot sequence)

**Status:** systemctl --user status knight.service (Print real time report of the service)

**Stop:** systemctl --user stop knight.service (Turns off the current running service, stops executable)

**IsActive:** systemctl --user is-active knight.service (returns activity of the service)

## Contributors

This project was designed and primarily developed by Quennel Kempe. Additional early contributions:

@Kaidokawaii – Wrote an early Python prototype for LED control, later rewritten in C to drive RGB LEDs (replacing the original LED-strip approach).

Mohammad Abumaali – Contributed initial servo control code early in the project, later substantially reworked.

Mark Madla – Assisted with 3D printing components for the exhibit.

Parker Stockdale – Assisted with wiring and deploying system.

Gustavo Gomez - Assisted in wiring RGB Leds.
