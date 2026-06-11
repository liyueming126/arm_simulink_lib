#include <stdint.h>
#include <string.h>

typedef int32_t q31_t;
typedef int64_t q63_t;

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

q31_t arm_pid_q31(
    arm_pid_instance_q31 * S,
    q31_t in)
{
    q63_t acc;
    q31_t out;

    acc = (q63_t)S->A0 * in;
    acc += (q63_t)S->A1 * S->state[0];
    acc += (q63_t)S->A2 * S->state[1];

    out = (q31_t)(acc >> 31u);
    out += S->state[2];

    S->state[1] = S->state[0];
    S->state[0] = in;
    S->state[2] = out;

    return out;
}
