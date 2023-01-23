#include "ev3_tacho.h"

int initialize_arm(int servo_port, uint8_t *sn_arm);
int lift_arm(uint8_t sn_arm, int arm_v);
int lower_arm(uint8_t sn_arm, int arm_v);