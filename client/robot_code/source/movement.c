#include <stdio.h>
#include <stdlib.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
#include "utils.c"

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

#define left_wheel_port 66
#define right_wheel_port 67

/*TODO:
- (optionnal) implement optimized operation for list such as storing the closest ray index
- be able to identify symetry groups in the collected ray data (need some experimentation to figure out the symetry groups of the ball, enemy robot and walls)
- identify the rays forming a symetry group
- (optionnal) apply some mapping transformation (interpolation, extrapolation, ...) in order to compensate for the acceleration phase of the motors ???
- pick the median ray index from within the rays subset
- get the corresponding angle (knowing that the robot angle isn't the same as it was when the sweep started)
- rotate the robot back to the chosen direction 
*/

/// @brief Collect Ray data and store it in the List given as parameter
void collect_and_store_ray(struct List* list) {
    struct Ray ray;

    int angle = 0; //TODO: use the compass value here
    float distance = 0.0; //TODO: right now get_sonar_value() is a void an requires a buffer parameter + include sensor.c

    initRay(&ray, distance, angle);
    put(&list, &ray);
}

/// @brief turns the robot of a given angle and (optionaly) collect rays data
/// @param angle angle (in degrees) of the sweep
/// @param scan if != 0, rays data will be collected
struct List turn_robot(int angle, int scan=0) {
    
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

    // Wait for the motors to finish
    while (get_tacho_state(left_wheel_port) != TACHO_STATE_HOLD && get_tacho_state(right_wheel_port) != TACHO_STATE_HOLD) {
        // Collect and store the current ray
        if (scan) collect_and_store_ray(&raysList);
        
        // Wait for POLLING_RATE ms before polling again
        Sleep(POLLING_RATE);
    }

    // Collect and store the final ray
    if (scan) collect_and_store_ray(&raysList);

    return raysList;
}

//whatever

static void _run_motor_forever(uint8_t sn_motor, int speed_sp)
{
    set_tacho_speed_sp(sn_motor, speed_sp);
    set_tacho_command_inx(sn_motor, TACHO_RUN_FOREVER);
}

static void _run_motor_timed(uint8_t sn_motor, int speed_sp, int time_sp)
{
    set_tacho_speed_sp(sn_motor, speed);
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

int main( void ) //TODO: this method is just for testing, the turn_method should only be called from the main.c script in the final version
{
    if ( ev3_init() == -1 ) return ( 1 );

    while ( ev3_tacho_init() < 1 ) Sleep( 1000 );  // Waiting for tachos being plugged

    printf( "*** ( EV3 ) Hello! ***\n" );

    int angle = 90; //example

    // Turn the robot around and collect rays while doing so
    struct List rays;
    rays = turn_robot(angle);
    
    // TODO: Process the rays here
    
    // Once we're done processing the rays and identifiying objects, clear the list to free memory
    clear(rays);
    
    return 0;
}
