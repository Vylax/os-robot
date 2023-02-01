#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
#include "ev3_sensor.h"
#include "ball.h"
#include "sensors.h"
#include "movement.h"
#include "init.h"
#include "utils.h"

// WIN32 /////////////////////////////////////////
#ifdef __WIN32__

#include <windows.h>

// UNIX //////////////////////////////////////////
#else

#include <unistd.h>
#define Sleep(msec) usleep((msec)*1000)
//////////////////////////////////////////////////
#endif

enum {SONAR, GYRO, COLOR, TOUCH, COMPASS, LEFT_MOTOR, RIGHT_MOTOR, ARM, HAND};

uint8_t components[9]  = {-1, -1, -1, -1, -1, -1, -1, -1, -1};

void stop_handler()
{
    //stop(TACHO_COAST);
    exit(0);
}

/// @brief Move forward and get the ball
void test1()
{   /*
    const int SPEED = 300;
    int distance, t;
    distance = 350; //mm

    get_value_sonar(&distance);
    t = cal_run_time(distance, SPEED);
    move_timed(-SPEED, t);
    Sleep(t);
    //grab_routine
    grab_ball();
    */
   return;
}

/// @brief Shoot a ball already in hand to the basket
void test2()
{
    //TODO: remove later
    //move_forever(200);
    //Sleep(3000);
    //grab_ball();
    //reload();
    //shoot_ball();
    return;
}

/// @brief Scan the environment to find the ball and go get it
void test3()
{
    //turn_robot();
    //test1();
    return;
}

/// @brief Scan the environment to find the ball, go get it and throw it
void test4()
{
    //test3();
    //test2();
    return;
}

void test5 ()
{
    return;
}

/// @brief Behave as an attacker
void attacker()
{
    int speed_forward, speed_backward, speed_turning;
    int t1, t2, t3, t4, t5, t6, t7;

    speed_forward = 400;
    speed_backward = 300;
    speed_turning = 200;

    // Todo: implement the strategy
    // 1)from the start position, move forward for 65cm 
    t1 = 200;
    move_timed(speed_forward, t1);
    
    // 2)turn right to +90 
    t2 = 200;
    turn_right(speed_turning, t2);
    
    // 3)move backward for 40cm 
    t3 = 200;
    move_timed(speed_forward, t3);
    
    // (get the 1st ball) 
    
    
    // 4) turn right to +180 
    t4 = 200;
    turn_right(speed_turning, t4);
    
    // 5) move backward for 50cm 
    t5 = 200;
    move_timed(speed_forward, t5);
    
    // (get the second ball) 
    

    // 6)  moving forward 60cm (to the shooting point) 
    t6 = 200;
    move_timed(speed_forward, t6);

    // 7) turn left to 0 (shoot)
    t7 = 400;
    turn_left(speed_turning, t7);
}

/// @brief  Behave as an defender
void defender()
{
    // Todo: implement the startegy
}


int main(int argc, char const *argv[])
{
    /* Checks arguments */
    if(argc != 2) {
        printf("Error: give name of the test you want to perform!\n");
        return -1;
    }
    /* Initialize sensors & motors */
    robot_init();
    /* Should print all the corrispondent ports */
    for (int i = 0; i < 9; i++)
    {
        printf("components[%d]: %d\n", i, components[i]);
    }
    
    // movement_test()
    if (!strcmp("movement", argv[1])) movement_test();
    // test1()
    if (!strcmp("test1", argv[1])) test1();
    // test2()
    if (!strcmp("test2", argv[1])) test2();
    // test3()
    if (!strcmp("test3", argv[1])) test3();
    // test4()
    if (!strcmp("test4", argv[1])) test4();
    // test5()
    if (!strcmp("test5", argv[1])) test5();
    // attacker()
    if (!strcmp("attacker", argv[1])) attacker();
    // defender()
    if (!strcmp("defender", argv[1])) defender();
     
    /* Uninitialize the robot */
    robot_uninit();
    return 0;
}