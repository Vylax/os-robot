#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
#include "ev3_sensor.h"
#include "movement.h"
#include "utils.h"
#include "sensors.h"

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
#define PI 3.14159
#define POLLING_RATE 10
#define BALL_RADIUS 25
#define AXIS_LENGTH 150

enum {SONAR, GYRO, COLOR, TOUCH, COMPASS, LEFT_MOTOR, RIGHT_MOTOR, ARM, HAND};

/// @brief Collect Ray data and store it in the List given as parameter
void collect_and_store_ray(List* list) {
    Ray ray;

    int angle = get_value_compass(components[COMPASS]);
    int distance = get_value_sonar(components[SONAR]);
    //printf("Compass value = %d, Sonar value = %d\n", angle, distance); //debug
    initRay(&ray, distance, angle);
    update_with_offset(&ray);
    put(list, &ray);
}

/// @brief turns the robot of a given angle and (optionaly) collect rays data
/// @param angle angle (in degrees) of the sweep
/// @param scan if != 0, rays data will be collected
List turn_robot(int angle, int scan) {
    int radiants = (angle * PI) / 180;
    
    // Reset sonar
    reset_sonar();

    // Initialise rays collection (if we don't scan we'll just return an empty list and ignore its value anyways)
    List raysList;
    init(&raysList);

    // Collect and store the initial ray
    if (scan) collect_and_store_ray(&raysList);

    // Set the motors to "coast" mode
    set_tacho_stop_action_inx(components[LEFT_MOTOR], TACHO_COAST);
    set_tacho_stop_action_inx(components[RIGHT_MOTOR], TACHO_COAST);

    float rotation_distance = PI * AXIS_LENGTH * (angle / 360.00); //2 * PI * (AXIS_LENGTH/2) * (angle / 360)
    float wheel_rotations = rotation_distance / (2 * PI * WHEEL_RADIUS);
    printf("Wheel rotations: %f\n", wheel_rotations);

    // Calculate the number of degrees to rotate each wheel
    int left_degrees = (int)(-1 * wheel_rotations * 360);
    int right_degrees = (int)(wheel_rotations * 360);

    // Set the motors to rotate to the calculated number of degrees
    set_tacho_position_sp(components[LEFT_MOTOR], left_degrees);
    set_tacho_position_sp(components[RIGHT_MOTOR], right_degrees);

    // Start the motors
    set_tacho_command_inx(components[LEFT_MOTOR], TACHO_RUN_TO_REL_POS);
    set_tacho_command_inx(components[RIGHT_MOTOR], TACHO_RUN_TO_REL_POS);

    char * state = malloc(sizeof(char) * 20);
    
    // Wait for the motors to finish
    while (get_tacho_state(components[LEFT_MOTOR], state, (size_t)20) != 1 || get_tacho_state(components[RIGHT_MOTOR], state, (size_t)20) != 1) {
        //printf("Inside loop... state=%s\n", state); //DEBUG
        
        // Collect and store the current ray
        if (scan) collect_and_store_ray(&raysList);
        
        // Wait for POLLING_RATE ms before polling again
        Sleep(POLLING_RATE);
    }

    printf("After while\n");

    free(state);

    // Collect and store the final ray
    if (scan) collect_and_store_ray(&raysList);

    return raysList;
}

/// @brief Identifie a ball position, and turn the robot towards it <br/> Returns the distance of the robot center of mass from the ball or -1 if no ball was found
/// @param raysList 
int turn_to_ball(List* raysList) {
    // Initialize variables to keep track of the current streak of rays
    int streakStart = -1;
    int streakEnd = -1;
    int streakMinDist = -1;
    int streakMaxDist = -1;

    // Iterate through all the rays in the list
    int i = 0;
    printf("Debug: i = %d\n", i);   //DEBUG
    while(i < raysList->size){
        Ray currentRay = raysList->data[i];

        // Check if the current ray is part of a streak
        if (streakStart >= 0 && streakEnd == i - 1 && abs(currentRay.distance - streakMinDist) <= BALL_RADIUS && abs(currentRay.distance - streakMaxDist) <= BALL_RADIUS) {
            streakEnd = i;
            // Update the min and max distance for the current streak
            streakMinDist = currentRay.distance < streakMinDist ? currentRay.distance : streakMinDist;
            streakMaxDist = currentRay.distance > streakMaxDist ? currentRay.distance : streakMaxDist;
        } else {
            // If the current ray is not part of a streak, check if the previous streak is valid
            if (streakEnd != -1 && streakStart < streakEnd) {
                printf("Found potential ball at angles [%d, %d]\n", raysList->data[streakStart].angle, raysList->data[streakEnd].angle);
                
                double alpha = abs(raysList->data[streakStart].angle - raysList->data[streakEnd].angle) / 2.0 * PI / 180.0; // Note: alpha is in radians
                printf("DEBUG: pot. ball alpha = %f, streakMin = %d, streakMax = %d\n", alpha, streakMinDist, streakMaxDist); //DEBUG
                double object_radius = streakMaxDist * sin(alpha);  //CHECK
                
                // Check if the object we found is a ball
                if(BALL_RADIUS/3.0 <= object_radius && object_radius <= BALL_RADIUS)
                    break;
                printf("The object found at angles [%d, %d] is not recognized as a ball\n", raysList->data[streakStart].angle, raysList->data[streakEnd].angle);
            }

            // Start a new streak
            streakStart = i;
            streakEnd = i;
            streakMinDist = currentRay.distance;
            streakMaxDist = currentRay.distance;
        }
        i++;
    }

    // Check if the last streak is valid
    if (streakEnd != -1 && streakStart < streakEnd) {
        printf("Found ball at angles [%d, %d]\n", raysList->data[streakStart].angle, raysList->data[streakEnd].angle);
        
        // Compute target angle
        int target_angle = abs(raysList->data[streakStart].angle + raysList->data[streakEnd].angle)/2; // Note: this is not the same value as alpha
        printf("Located ball: target_angle = %d\n", target_angle);
        //DEBUG
        //int target_angle = (raysList->data[streakStart].angle + raysList->data[streakEnd].angle)/2 + 180;
        //if(target_angle >= 360) target_angle -= 360;

        // Rotate the robot toward the ball
        (void) turn_robot(target_angle, 0);
        
        return streakMinDist + BALL_RADIUS;
    }
    return -1;
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
    _run_motor_forever(components[LEFT_MOTOR], speed_sp);
    _run_motor_forever(components[RIGHT_MOTOR], speed_sp);
}

/// @brief keeps two motors moving at a speed for a period of time
void move_timed(int speed_sp, int time_sp)
{
    if (speed_sp == 0 || time_sp == 0)
        return;
    _run_motor_timed(components[LEFT_MOTOR], speed_sp, time_sp);
    _run_motor_timed(components[RIGHT_MOTOR], speed_sp, time_sp);
}

void move_wheel_timed(int speed_sp, int time_sp, int flag)
{
    if (speed_sp == 0 || time_sp == 0)
        return;
    if(flag == 0)
        _run_motor_timed(components[LEFT_MOTOR], speed_sp, time_sp);
    else 
        _run_motor_timed(components[RIGHT_MOTOR], speed_sp, time_sp);
}

/// @brief Used as a test of movement functionalities
void movement_test()
{
    printf("\n\n-- STARTING MOVEMENT TEST --\n\n");
    int angle = 90;

    //Setting speeds (If turning speed is too high, the rotation is not smooth. i.e. don't exceed 200)
    set_tacho_speed_sp(components[LEFT_MOTOR], 150);
    set_tacho_speed_sp(components[RIGHT_MOTOR], 150);

    // Turn the robot around and collect rays while doing so
    List rays;
    rays = turn_robot(angle, 1);
    
    // Process the rays to find a ball, and set robot direction towards it if one is found and get the distance to the ball (-1 if there is no ball)
    int dist_to_ball = turn_to_ball(&rays);

    // Once we're done processing the rays and identifiying objects, clear the list to free memory
    clear(&rays);
    
    // Example code of what to do next
    if(dist_to_ball >= 0) {
        printf("Moving forward %d cm to reach the ball\n", dist_to_ball);
        // Move forward dist_to_ball cm here
    }    
    printf("\n\n-- END OF MOVEMENT TEST --\n\n");
    return;
}