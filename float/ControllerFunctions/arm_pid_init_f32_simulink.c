#include <stdint.h>
#include <string.h>

typedef struct
{
    float A0;
    float A1;
    float A2;
    float state[3];
    float Kp;
    float Ki;
    float Kd;
} arm_pid_instance_f32;

void arm_pid_init_f32(
    arm_pid_instance_f32 * S,
    int32_t resetStateFlag)
{
    /* Derived coefficient A0 */
    S->A0 = S->Kp + S->Ki + S->Kd;

    /* Derived coefficient A1 */
    S->A1 = (-S->Kp) - ((float)2.0 * S->Kd);

    /* Derived coefficient A2 */
    S->A2 = S->Kd;

    /* Check whether state needs reset or not */
    if (resetStateFlag)
    {
        memset(S->state, 0, 3u * sizeof(float));
    }
}