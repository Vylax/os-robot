#NOTE: when compiling inside ev3 robot, use CC=gcc; when cross compiling, substitute CC with arm-linux-gnueabi-gcc
CC 			= gcc
CFLAGS 		= -O2 -g -std=gnu99 -W -Wall -Wno-comment
INCLUDES 	= -I./ev3dev-c/source/ev3 -I./include/
LDFLAGS 	= -L./libraries -lrt -lm -lev3dev-c -lpthread
BUILD_DIR 	= ./build
SOURCE_DIR 	= ./source

OBJS = \
	$(BUILD_DIR)/utils.o \
	$(BUILD_DIR)/init.o \
	$(BUILD_DIR)/sensors.o \
	$(BUILD_DIR)/movement.o \
	$(BUILD_DIR)/ball.o \
	$(BUILD_DIR)/main.o

all: main

main: ${OBJS}
	$(CC) $(INCLUDES) $(CFLAGS) $(OBJS) $(LDFLAGS) -o main

#$(OBJS): $(BUILD_DIR)

#$(BUILD_DIR):
#	mkdir $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c
	$(CC) -c $(SOURCE_DIR)/$*.c $(INCLUDES) $(CFLAGS) -o $(BUILD_DIR)/$*.o

clean:
	rm -f $(BUILD_DIR)/*.o
	rm -f ./main
