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

static inline q31_t clip_q63_to_q31(q63_t x)
{
    return ((q31_t)(x >> 32) != ((q31_t)x >> 31))
        ? ((int32_t)0x7FFFFFFF ^ ((q31_t)(x >> 63)))
        : (q31_t)x;
}

void arm_pid_init_q31(
    arm_pid_instance_q31 * S,
    int32_t resetStateFlag)
{
    q31_t temp;

    temp = clip_q63_to_q31((q63_t)S->Kp + S->Ki);
    S->A0 = clip_q63_to_q31((q63_t)temp + S->Kd);

    temp = clip_q63_to_q31((q63_t)S->Kd + S->Kd);
    S->A1 = -clip_q63_to_q31((q63_t)temp + S->Kp);

    S->A2 = S->Kd;

    if (resetStateFlag)
    {
        memset(S->state, 0, 3u * sizeof(q31_t));
    }
}
