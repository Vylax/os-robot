#include <stdio.h>
#include <stdlib.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
#include "ev3_sensor.h"
// WIN32 /////////////////////////////////////////
#ifdef __WIN32__

#include <windows.h>

// UNIX //////////////////////////////////////////
#else

#include <unistd.h>
#define Sleep( msec ) usleep(( msec ) * 1000 )
#define PI 3.142857
#define WHEEL_DIAM 55 //Wheels' diameter is 5.5 cm

//////////////////////////////////////////////////
#endif
const char const *color[] = { "?", "BLACK", "BLUE", "GREEN", "YELLOW", "RED", "WHITE", "BROWN" };
#define COLOR_COUNT  (( int )( sizeof( color ) / sizeof( color[ 0 ])))

static bool _check_pressed( uint8_t sn )
{
  int val;

  if ( sn == SENSOR__NONE_ ) {
    return ( ev3_read_keys(( uint8_t *) &val ) && ( val & EV3_KEY_UP ));
  }
  return ( get_sensor_value( 0, sn, &val ) && ( val != 0 ));
}

int main( void )
{
  // int i;
  uint8_t sn_left;
  uint8_t sn_right;
  uint8_t sn_arm;
  uint8_t sn_hand;
  // FLAGS_T state;
  uint8_t sn_touch;
  uint8_t sn_color;
  uint8_t sn_gyro;
  uint8_t sn_sonar;
  // uint8_t sn_mag;
  // char s[ 256 ];
  // int val;
  int count_per_rot;
  int d = 5;
  int ball_found = 0;
  int max_speed;
  int dc_port = 65;
  int left_wheel_port = 66;
  int right_wheel_port = 67;
  int servo_port = 68;
  int val;
  float value;
  float distance;
  float current_speed;
  float t;
  float speed_ratio = 0.03;
  // uint32_t n, ii;
#ifndef __ARM_ARCH_4T__
  /* Disable auto-detection of the brick (you have to set the correct address below) */
  ev3_brick_addr = "10.192.59.99";

#endif
  if ( ev3_init() == -1 ) return ( 1 );

#ifndef __ARM_ARCH_4T__
  printf( "The EV3 brick auto-detection is DISABLED,\nwaiting %s online with plugged tacho...\n", ev3_brick_addr );

#else
  printf( "Waiting tacho is plugged...\n" );

#endif

  while ( ev3_tacho_init() < 1 ) Sleep( 1000 );  // Waiting for tachos being plugged

  printf( "*** ( EV3 ) Hello! ***\n" );

  // The left wheel
  if ( ev3_search_tacho_plugged_in(left_wheel_port, 0, &sn_left, 0 )) {

    printf( "LEGO_EV3_M_MOTOR (LEFT) is found...\n" );
    get_tacho_max_speed( sn_left, &max_speed );
    printf("  max speed = %d\n", max_speed );
    get_tacho_count_per_rot(sn_left, &count_per_rot);
    printf("  count_per_rot = %d\n", count_per_rot );
    set_tacho_stop_action_inx( sn_left, TACHO_COAST );
    current_speed = max_speed * speed_ratio;

  } else {
    printf( "LEGO_EV3_M_MOTOR (LEFT) is NOT found\n" );
  }
  
  // The right wheel
  if ( ev3_search_tacho_plugged_in(right_wheel_port, 0, &sn_right, 0 )) {

    printf( "LEGO_EV3_M_MOTOR (RIGHT) is found...\n" );
    get_tacho_max_speed( sn_right, &max_speed );
    printf("  max speed = %d\n", max_speed );
    get_tacho_count_per_rot(sn_left, &count_per_rot);
    printf("  count_per_rot = %d\n", count_per_rot );
    set_tacho_stop_action_inx( sn_right, TACHO_COAST );
    
  } else {
    printf( "LEGO_EV3_M_MOTOR (RIGHT) is NOT found\n" );
  }

    if ( ev3_search_sensor( LEGO_EV3_COLOR, &sn_color, 0 )) {
      printf( "COLOR sensor is found, reading COLOR...\n" );
      if ( !get_sensor_value( 0, sn_color, &val ) || ( val < 0 ) || ( val >= COLOR_COUNT )) {
        val = 0;
      }
      printf( "\r(%s) \n", color[ val ]);
      fflush( stdout );
    }

    if (ev3_search_sensor(LEGO_EV3_US, &sn_sonar,0)){
      printf("SONAR found, reading sonar...\n");
      if ( !get_sensor_value0(sn_sonar, &value )) {
        value = 0;
      }
      printf( "\r(%f) \n", value);
      fflush( stdout );
    }

    if ( !ev3_search_sensor(LEGO_EV3_GYRO, &sn_gyro, 0)) {
        printf("GYRO found, reading gyro...\n");
        if ( !get_sensor_value0(sn_gyro, &value )) {
            value = 0;
        }
        printf( "\r(%f) \n", value);
        // fflush( stdout );
    } else {
        printf("GYRO is not found\n");
    }
    current_speed = max_speed * speed_ratio;
    while (d < 60)
    {
        printf("%d\n", d);
        set_tacho_speed_sp( sn_right, - (current_speed + d ));
        set_tacho_speed_sp( sn_left,  current_speed + d );
        set_tacho_time_sp( sn_left, 1000 );
        set_tacho_time_sp( sn_right, 1000 );
        set_tacho_command_inx( sn_left, TACHO_RUN_TIMED );
        set_tacho_command_inx( sn_right, TACHO_RUN_TIMED );
        get_sensor_value0( sn_sonar, &distance );
        
        Sleep(10000);
        if ( !get_sensor_value( 0, sn_color, &val ) || ( val < 0 ) || ( val >= COLOR_COUNT )) {
            val = 0;
        } else {
            printf( "\r %d(%s) \n", val, color[ val ]);
        }

        if ((val > 0) || ( distance < 500 )){
            get_sensor_value0( sn_sonar, &distance );
            t = (float) (count_per_rot * (abs(distance) + WHEEL_DIAM) )/(current_speed * PI * WHEEL_DIAM) ;
            printf("Tachos are going to run for %f seconds at a  \n", t);
            set_tacho_speed_sp( sn_left, current_speed );
            set_tacho_speed_sp( sn_right, current_speed );
            set_tacho_time_sp( sn_left, t * 1000 );
            set_tacho_time_sp( sn_right, t * 1000 );
            set_tacho_command_inx( sn_left, TACHO_RUN_TIMED );
            set_tacho_command_inx( sn_right, TACHO_RUN_TIMED );
            Sleep(t * 1000 );

            if ( ev3_search_tacho_plugged_in(dc_port, 0, &sn_arm, 0 )) {

                printf( "LEGO_EV3_M_MOTOR (ARM) is found...\n" );
                get_tacho_max_speed( sn_arm, &max_speed );
                printf("  max speed = %d\n", max_speed );
                get_tacho_count_per_rot(sn_arm, &count_per_rot);
                printf("  count_per_rot = %d\n", count_per_rot );
                set_tacho_stop_action_inx( sn_arm, TACHO_COAST );
                printf("Raising the arm now...\n");
                set_tacho_speed_sp( sn_arm, max_speed / 50 );
                set_tacho_time_sp( sn_arm, 5000 ); 
                set_tacho_command_inx( sn_arm, TACHO_RUN_TIMED );    
            } else {
                printf( "LEGO_EV3_M_MOTOR (ARM) is NOT found\n" );
            }

            if ( ev3_search_tacho_plugged_in(servo_port, 0, &sn_hand, 0 )) {

                printf( "LEGO_EV3_M_MOTOR (HAND) is found...\n" );
                get_tacho_max_speed( sn_hand, &max_speed );
                printf("  max speed = %d\n", max_speed );
                get_tacho_count_per_rot(sn_hand, &count_per_rot);
                printf("  count_per_rot = %d\n", count_per_rot );
                set_tacho_stop_action_inx( sn_hand, TACHO_COAST );
                printf("Fetching the ball now...\n");
                set_tacho_speed_sp( sn_hand, - max_speed / 50 );
                set_tacho_time_sp( sn_hand, 10000 ); 
                set_tacho_command_inx( sn_hand, TACHO_RUN_TIMED );   

                Sleep(7000); 

            } else {
                printf( "LEGO_EV3_M_MOTOR (HAND) is NOT found\n" );
            }
            set_tacho_speed_sp( sn_arm, - max_speed / 50 );
            set_tacho_time_sp( sn_arm, 5000 ); 
            set_tacho_command_inx( sn_arm, TACHO_RUN_TIMED );   
            break;
        }
        d = d + 5;
    }
        

  return ( 0 );
}