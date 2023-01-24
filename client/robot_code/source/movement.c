#include <stdio.h>
#include <stdlib.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
#include "utils.c"
#include "sensors.c" // TODO: remove later on
#include "../include/sensors.h"

#ifndef __ARM_ARCH_4T__
    /* Disable auto-detection of the brick (you have to set the correct address below) */
    ev3_brick_addr = "10.192.59.99";
#endif

// WIN32 /////////////////////////////////////////
#ifdef __WIN32__
#include <windows.h>
// UNIX //////////////////////////////////////////
#else
#include <unistd.h>
#define Sleep(msec) usleep((msec)*1000)
//////////////////////////////////////////////////
#endif

//Constants declaration
#define WHEEL_RADIUS 27.5
#define PI 3.142857
#define POLLING_RATE 10
#define BALL_RADIUS 5 // TODO: get the actual value

#define left_wheel_port 66
#define right_wheel_port 67
#define SONAR_PORT 3
#define COMPASS_PORT 0

int ports[9];

/*TODO:
- (optionnal) apply some mapping transformation (interpolation, extrapolation, ...) in order to compensate for the acceleration phase of the motors ???
- rotate the robot back to the chosen direction 
*/

/// @brief Collect Ray data and store it in the List given as parameter
void collect_and_store_ray(struct List* list) {
    struct Ray ray;

    int angle = get_value_compass(COMPASS_PORT);
    int distance = get_value_sonar(SONAR_PORT);

    initRay(&ray, distance, angle);
    update_with_offset(&ray);
    put(list, &ray);
}

/*void identify_ball() {
    int firstStreakIndex = -1;
    int firstStreakDist = -1;
    // TODO: implement List structure for integers
    
    for (int i = 0; i < list->size - 1; i++) {
        list->data[i].
    }
}*/

/// @brief turns the robot of a given angle and (optionaly) collect rays data
/// @param angle angle (in degrees) of the sweep
/// @param scan if != 0, rays data will be collected
struct List turn_robot(int angle, int scan) {
    
    // Initialise sensors
    reset_sonar(SONAR_PORT);

    // Initialise rays collection (if we don't scan we'll just return an empty list and ignore its value anyways)
    struct List raysList;
    init(&raysList);

    // Collect and store the initial ray
    if (scan) collect_and_store_ray(&raysList);

    // Set the motors to "coast" mode
    set_tacho_stop_action_inx(left_wheel_port, TACHO_COAST);
    set_tacho_stop_action_inx(right_wheel_port, TACHO_COAST);

    // Calculate the number of degrees to rotate each wheel
    float wheel_rotations = angle / (2 * PI * WHEEL_RADIUS);
    int left_degrees = (int)(-1 * wheel_rotations * 360);
    int right_degrees = (int)(wheel_rotations * 360);

    // Set the motors to rotate to the calculated number of degrees
    set_tacho_position_sp(left_wheel_port, left_degrees);
    set_tacho_position_sp(right_wheel_port, right_degrees);

    // Start the motors
    set_tacho_command_inx(left_wheel_port, TACHO_RUN_TO_REL_POS);
    set_tacho_command_inx(right_wheel_port, TACHO_RUN_TO_REL_POS);

	
    char * state = malloc(sizeof(char) * 20);

    // Wait for the motors to finish
    while (get_tacho_state(left_wheel_port, state, (size_t)20) != TACHO_STATE_HOLD || get_tacho_state(right_wheel_port, state, (size_t)20) != TACHO_STATE_HOLD) {
        // Collect and store the current ray
        if (scan) collect_and_store_ray(&raysList);
        
        // Wait for POLLING_RATE ms before polling again
        Sleep(POLLING_RATE);
    }

    free(state);

    // Collect and store the final ray
    if (scan) collect_and_store_ray(&raysList);

    return raysList;
}

/// @brief Identifie entity possible angles <br/> Needs to be combine with other functions to be complete
/// @param raysList 
/// @param ball_detection_threshold 
void identify_entity(struct List* raysList, int detection_threshold) {
    // Initialize variables to keep track of the current streak of rays
    int streakStart = -1;
    int streakEnd = -1;
    int streakMinDist = -1;
    int streakMaxDist = -1;

    // Iterate through all the rays in the list
    int i = 0;
    while(i < raysList->size){
        struct Ray currentRay = raysList->data[i];

        // Check if the current ray is part of a streak
        if (streakStart >= 0 && streakEnd == i - 1 && abs(currentRay.distance - streakMinDist) <= detection_threshold && abs(currentRay.distance - streakMaxDist) <= detection_threshold) {
            streakEnd = i;
            // Update the min and max distance for the current streak
            streakMinDist = currentRay.distance < streakMinDist ? currentRay.distance : streakMinDist;
            streakMaxDist = currentRay.distance > streakMaxDist ? currentRay.distance : streakMaxDist;
        } else {
            // If the current ray is not part of a streak, check if the previous streak is valid
            if (streakEnd != -1 && streakStart < streakEnd) {
                printf("Found potential ball at angles [%d, %d]\n", raysList->data[streakStart].angle, raysList->data[streakEnd].angle);
            }
            i=streakStart+1;

            // Start a new streak
            streakStart = i;
            streakEnd = i;
            streakMinDist = currentRay.distance;
            streakMaxDist = currentRay.distance;
        }
    }
    // Check if the last streak is valid
    if (streakEnd != -1 && streakStart < streakEnd) {
        printf("Found potential ball at angles [%d, %d]\n", raysList->data[streakStart].angle, raysList->data[streakEnd].angle);
    }
}

static void _run_motor_forever(uint8_t sn_motor, int speed_sp)
{
    set_tacho_speed_sp(sn_motor, speed_sp);
    set_tacho_command_inx(sn_motor, TACHO_RUN_FOREVER);
}

static void _run_motor_timed(uint8_t sn_motor, int speed_sp, int time_sp)
{
    set_tacho_speed_sp(sn_motor, speed_sp);
    set_tacho_time_sp( sn_motor, time_sp);
    set_tacho_command_inx(sn_motor, TACHO_RUN_TIMED);
}

/// @brief keeps two motors moving forever
void move_forever(int speed_sp)
{
    if (speed_sp == 0)
        return;
    _run_motor_forever(left_wheel_port, speed_sp);
    _run_motor_forever(right_wheel_port, speed_sp);
}

/// @brief keeps two motors moving at a speed for a period of time
void move_timed(int speed_sp, int time_sp)
{
    if (speed_sp == 0 || time_sp == 0)
        return;
    _run_motor_timed(left_wheel_port, speed_sp, time_sp);
    _run_motor_timed(right_wheel_port, speed_sp, time_sp);
}

// TODO: this method is just for testing, the turn_method should only be called from the main.c script in the final version
int main( void ) 
{
    if ( ev3_init() == -1 ) return ( 1 );

    while ( ev3_tacho_init() < 1 ) Sleep( 1000 );  // Waiting for tachos being plugged

    printf( "*** ( EV3 ) Hello! ***\n" );

    int angle = 90; // example

    // Turn the robot around and collect rays while doing so
    struct List rays;
    rays = turn_robot(angle, 1);
    
    // TODO: Process the rays here
    identify_entity(&rays, BALL_RADIUS);

    // Once we're done processing the rays and identifiying objects, clear the list to free memory
    clear(&rays);
    
    return 0;
}
