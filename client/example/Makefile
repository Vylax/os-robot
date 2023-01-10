drive:
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c drive.c -o drive.o
	gcc drive.o -Wall -lm -lev3dev-c -o drive
	./drive

sensor:
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c sensor.c -o sensor.o
	gcc sensor.o -Wall -lm -lev3dev-c -o sensor
	./sensor

tacho:
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c tacho.c -o tacho.o
	gcc tacho.o -Wall -lm -lev3dev-c -o tacho
	./tacho

