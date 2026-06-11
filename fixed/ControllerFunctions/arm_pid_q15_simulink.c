#include <stdint.h>
#include <string.h>

typedef int16_t q15_t;
typedef int64_t q63_t;

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

q15_t arm_pid_q15(
    arm_pid_instance_q15 * S,
    q15_t in)
{
    q63_t acc;
    q15_t out;

    acc = (q63_t)((int32_t)S->A0 * (int32_t)in);
    acc += (int32_t)S->A1 * (int32_t)S->state[0];
    acc += (int32_t)S->A2 * (int32_t)S->state[1];
    acc += (int32_t)S->state[2] << 15;

    out = __SSAT((int32_t)(acc >> 15), 16);

    S->state[1] = S->state[0];
    S->state[0] = in;
    S->state[2] = out;

    return out;
}
