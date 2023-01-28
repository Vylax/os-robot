#include "ev3.h"

/// @brief Collect Ray data and store it in the List given as parameter
void collect_and_store_ray(struct List* list, uint8_t* components);

/// @brief turns the robot of a given angle and (optionaly) collect rays data
/// @param angle angle (in degrees) of the sweep
/// @param scan if != 0, rays data will be collected
struct List turn_robot(int angle, int scan, uint8_t* components);

/// @brief Identify a ball position, and turn the robot towards it <br/> Returns the distance of the robot center of mass from the ball or -1 if no ball was found
/// @param raysList 
int turn_to_ball(struct List* raysList, uint8_t* components);

/// @brief keeps two motors moving forever
void move_forever(int speed_sp, uint8_t* components);

/// @brief keeps two motors moving at a speed for a period of time
void move_timed(int speed_sp, int time_sp, uint8_t* components);

/// @brief Used as a test of movement functionalities
void movement_test(uint8_t* components);