# Executable files should be put in /build
# Source files should be put in /source
# Header files should be put in /include
# Libraries should be put in /libraries
CC 			= arm-linux-gnueabi-gcc
CFLAGS 		= -O2 -g -std=gnu99 -W -Wall -Wno-comment
INCLUDES 	= -I./ev3dev-c/source/ev3 -I./include/
LDFLAGS 	= -L./libraries -lrt -lm -lev3dev-c -lpthread -lsensors -lutils -lmovement -linit -lball
BUILD_DIR 	= ./build
LIB_DIR		= ./libraries
SOURCE_DIR 	= ./source
INCLUDE_DIR = ./include
LIBS  = $(LIB_DIR)/libmovement.so \
		$(LIB_DIR)/libsensors.so \
		$(LIB_DIR)/libinit.so \
		$(LIB_DIR)/libutils.o \
		$(LIB_DIR)/libball.o \


all: main

#NOTE: all tests should be implemented inside main.c
main: ${LIBS}
	$(CC) $(INCLUDES) $(CFLAGS) $(LDFLAGS) -c $(BUILD_DIR)/main.o -o main

#Generate all obj files neede to build each final executable
$(BUILD_DIR)/main.o: $(SOURCE_DIR)/main.c
	$(CC) -c $(SOURCE_DIR)/main.c $(INCLUDES) -o $(BUILD_DIR)/main.o


#Create the libraries (updated)
$(LIB_DIR)/libsensors.so: $(SOURCE_DIR)/sensors.c $(INCLUDE_DIR)/sensors.h
    gcc -shared -fPIC -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wno-comment -c $(SOURCE_DIR)/sensors.c -lm -lev3dev-c -o $(LIB_DIR)/libsensors.so

$(LIB_DIR)/libutils.so: $(SOURCE_DIR)/utils.c $(INCLUDE_DIR)/utils.h
    gcc -shared -fPIC -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wno-comment -c $(SOURCE_DIR)/utils.c -lm -lev3dev-c -o $(LIB_DIR)/libutils.so

$(LIB_DIR)/libinit.so: $(SOURCE_DIR)/init.c $(INCLUDE_DIR)/init.h
    gcc -shared -fPIC -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wno-comment -c $(SOURCE_DIR)/init.c -lm -lev3dev-c -o $(LIB_DIR)/libinit.so

$(LIB_DIR)/libball.so: $(SOURCE_DIR)/ball.c $(INCLUDE_DIR)/ball.h
    gcc -shared -fPIC -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wno-comment -c $(SOURCE_DIR)/ball.c -lm -lev3dev-c -o $(LIB_DIR)/libball.so

$(LIB_DIR)/libmovement.so: $(SOURCE_DIR)/movement.c $(INCLUDE_DIR)/movement.h $(LIB_DIR)/libsensors.so
	gcc -shared -fPIC -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wno-comment -c $(SOURCE_DIR)/movement.c -lm -lev3dev-c -o $(LIB_DIR)/libmovement.so

#NOTE: should be checked
clean:
	rm -f $(BUILD_DIR)/*.o
	rm ./main
