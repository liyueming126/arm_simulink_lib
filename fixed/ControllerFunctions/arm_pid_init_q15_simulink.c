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

static inline int16_t __SSAT(int32_t val, int32_t sat)
{
    int32_t max = (1 << (sat - 1)) - 1;
    int32_t min = -(1 << (sat - 1));
    if (val > max) return (int16_t)max;
    if (val < min) return (int16_t)min;
    return (int16_t)val;
}

void arm_pid_init_q15(
    arm_pid_instance_q15 * S,
    int32_t resetStateFlag)
{
    int32_t temp;

    temp = (int32_t)S->Kp + (int32_t)S->Ki + (int32_t)S->Kd;
    S->A0 = __SSAT(temp, 16);

    temp = -((int32_t)S->Kd + (int32_t)S->Kd + (int32_t)S->Kp);
    S->A1 = __SSAT(temp, 16);
    S->A2 = S->Kd;

    if (resetStateFlag)
    {
        memset(S->state, 0, 3u * sizeof(q15_t));
    }
}
