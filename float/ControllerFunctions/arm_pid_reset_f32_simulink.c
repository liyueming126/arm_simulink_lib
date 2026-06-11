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

void arm_pid_reset_f32(
    arm_pid_instance_f32 * S)
{
    memset(S->state, 0, 3u * sizeof(float));
}