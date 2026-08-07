CC = gcc
CFLAGS = -Wall -Wextra -O2
LIBS = -lwiringPi -lgpiod

TARGET = robot_control
OBJS = main.o led.o audio.o pir.o servo.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LIBS)

main.o: main.c led.h audio.h pir.h servo.h
	$(CC) $(CFLAGS) -c main.c

led.o: led.c led.h
	$(CC) $(CFLAGS) -c led.c

audio.o: audio.c audio.h
	$(CC) $(CFLAGS) -c audio.c

pir.o: pir.c pir.h
	$(CC) $(CFLAGS) -c pir.c

servo.o: servo.c servo.h
	$(CC) $(CFLAGS) -c servo.c

clean:
	rm -f $(TARGET) core *.o

.PHONY: all clean
