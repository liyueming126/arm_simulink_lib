#include <stdint.h>
#include <string.h>

typedef int32_t q31_t;

typedef struct
{
    q31_t A0;
    q31_t A1;
    q31_t A2;
    q31_t state[3];
    q31_t Kp;
    q31_t Ki;
    q31_t Kd;
} arm_pid_instance_q31;

void arm_pid_reset_q31(
    arm_pid_instance_q31 * S)
{
    memset(S->state, 0, 3u * sizeof(q31_t));
}
