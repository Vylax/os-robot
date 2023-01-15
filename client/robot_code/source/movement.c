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

#define left_wheel_port 66
#define right_wheel_port 67

/*TODO:
= create a data structure to store rays data
= implement a dynamic array structure (List) because we can't tell for sure how many cycles the rotation of the robot can take (due to inconsistencies caused partly by the acceleration phase of motors)
- (optionnal) implement optimized operation for list such as storing the closest ray index
= collect rays at a given frequency
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
    float distance = 0.0; //TODO: implement something like "get_distance()" in sensors.c that gets the value from the sonar sensor

    initRay(&ray, distance, angle);
    put(&list, &ray);
}

void put(struct List* list, int value)

/// @brief turns the robot of a given angle and (optionaly) collect rays data
/// @param angle angle (in degrees) of the sweep
/// @param scan if != 0, rays data will be collected
struct List turn_robot(int angle, int scan=0) {
    
    //initialise rays collection (if we don't scan we'll just return an empty list and ignore its value anyways)
    struct List raysList;
    init(&raysList);

    //Collect and store the initial ray
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
        //Collect and store the current ray
        if (scan) collect_and_store_ray(&raysList);
        
        Sleep(10);//TODO: change this to the ray collecting period
    }

    //Collect and store the final ray
    if (scan) collect_and_store_ray(&raysList);

    return raysList;
}

int main( void ) //TODO: this method is just for testing, the turn_method should only be called from the main.c script in the final version
{
    if ( ev3_init() == -1 ) return ( 1 );

    while ( ev3_tacho_init() < 1 ) Sleep( 1000 );  // Waiting for tachos being plugged

    printf( "*** ( EV3 ) Hello! ***\n" );

    int angle = 90; //example

    (void) turn_robot(angle); //note: the void casting is unnecessary but more "ethical" 
    return 0;
}
