#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
#include "sensors.h"
#include "movements.h"
#include "utils.c"

#define left_wheel_port 66
#define right_wheel_port 67

void stop_handler()
{
    stop(TACHO_COAST);
    exit(0);
}

/// @brief Move forward and get the ball
void test1()
{
    const int SPEED = 800;
    int distance;

    get_sonar_value(&distance);
    t = cal_run_time(left_wheel_port, distance, SPEED);
    move_timed(SPEED, t);
    Sleep(t);
    // Todo
    grab_routine();
    
}

/// @brief Shoot a ball already in hand to the basket
void test2()
{
    // Todo
    aim_routine();
    // Todo
    throw_routine();
}

/// @brief Scan the environment to find the ball and go get it
void test3()
{
    turn_robot();

    test1();
}

/// @brief Scan the environment to find the ball, go get it and throw it
void test4()
{
    test3();
    test2();
}

/// @brief Behave as an attacker
void attacker()
{
    // Todo: implement the strategy
}

/// @brief  Behave as an defender
void defender()
{
    // Todo: implement the startegy
}

int main()
{
    // test1()

    // test2()

    // test3()

    // attacker()

    // defender()

}