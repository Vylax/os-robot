CC 			= arm-linux-gnueabi-gcc
CFLAGS 		= -O2 -g -std=gnu99 -W -Wall -Wno-comment
INCLUDES 	= -I./ev3dev-c/source/ev3 -I./include/
LDFLAGS 	= -L./libraries -lrt -lm -lev3dev-c -lpthread 
BUILD_DIR 	= ./build
SOURCE_DIR 	= ./source

OBJS = $(BUILD_DIR)/movement.o \
		$(BUILD_DIR)/sensors.o \
		$(BUILD_DIR)/main.o

all: main

main: ${OBJS}
	$(CC) $(INCLUDES) $(CFLAGS) $(OBJS) $(LDFLAGS) -o main

$(OBJS): $(BUILD_DIR)

test1:
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c ../test1.c -o ../test1.o
	gcc ../test1.o -Wall -lm -lev3dev-c -o ../test1
test2:
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c ../test2.c -o ../test2.o
	gcc ../test2.o -Wall -lm -lev3dev-c -o ../test2

robot:
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c ../tester.c -o ../tester.o
	gcc ../tester.o -Wall -lm -lev3dev-c -o ../tester
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c ../i2c.c -o ../i2c.o
	gcc ../i2c.o -Wall -lm -lev3dev-c -o ../i2c
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wno-comment -c source/sensors.c -o source/sensors.o
	gcc source/sensors.o -lm -lev3dev-c -o source/sensors
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wno-comment -c source/utils.c -o source/utils.o
	gcc source/utils.o -lm -lev3dev-c -o source/utils
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wno-comment -c source/movement.c -o source/movement.o
	gcc source/movement.o -lm -lev3dev-c -o source/movement
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wno-comment -c source/init.c -o source/init.o
	gcc source/init.o -lm -lev3dev-c -o source/init

#Implement later using shared library
sensors:
	gcc -shared -fPIC -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wno-comment -c source/sensors.c -o source/sensors.o
	gcc source/sensors.o -lm -lev3dev-c -shared -o libraries/libsensors.so	

utils:	
	gcc -shared -fPIC -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wno-comment -c source/utils.c -o source/utils.o
	gcc source/utils.o -lm -lev3dev-c -shared -o libraries/libutils.so	

movement:	
	gcc -shared -fPIC -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wno-comment -c source/utils.c -o source/utils.o
	gcc source/utils.o -lm -lev3dev-c -shared -o libraries/libutils.so	

init:	
	gcc -shared -fPIC -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wno-comment -c source/utils.c -o source/utils.o
	gcc source/utils.o -lm -lev3dev-c -shared -o libraries/libutils.so	
$(BUILD_DIR):
	mkdir $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c
	$(CC) -c $(SOURCE_DIR)/$*.c $(INCLUDES) -o $(BUILD_DIR)/$*.o

clean:
	rm -f $(BUILD_DIR)/*.o
	rm ./main
