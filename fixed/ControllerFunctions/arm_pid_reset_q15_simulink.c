#include <stdint.h>
#include <string.h>

typedef int16_t q15_t;

typedef struct
{
    q15_t A0;
    q15_t A1;
    q15_t A2;
    q15_t state[3];
    q15_t Kp;
    q15_t Ki;
    q15_t Kd;
} arm_pid_instance_q15;

void arm_pid_reset_q15(
    arm_pid_instance_q15 * S)
{
    memset(S->state, 0, 3u * sizeof(q15_t));
}
