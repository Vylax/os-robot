#ifndef HEADER_MOVEMENT
#define HEADER_MOVEMENT

#include "./utils.h"

extern uint8_t components[];

/// @brief Collect Ray data and store it in the List given as parameter
void collect_and_store_ray(List* list);

/// @brief turns the robot of a given angle and (optionaly) collect rays data
/// @param angle angle (in degrees) of the sweep
/// @param scan if != 0, rays data will be collected
List turn_robot(int angle, int scan);

/// @brief Identify a ball position, and turn the robot towards it <br/> Returns the distance of the robot center of mass from the ball or -1 if no ball was found
/// @param raysList 
int turn_to_ball(List* raysList);

/// @brief keeps two motors moving forever
void move_forever(int speed_sp);

/// @brief keeps two motors moving at a speed for a period of time
void move_timed(int speed_sp, int time_sp);

/// @
void move_wheel_timed(int speed_sp, int time_sp, int flag);

/// @brief turns the robot to its right
void turn_right(int speed_sp, int time_sp)

/// @brief turns the robot to its right
void turn_left(int speed_sp, int time_sp)
