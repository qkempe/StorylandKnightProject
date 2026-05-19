cd ~/
git clone https://github.com/sunfounder/raphael-kit.git --depth 1

to compile: gcc main.c audio.c -o robot_system -lwiringPi -lgpiod

to run: ./robot_system
