/* Simplified implementation of ARM Biquad DF2T init for Simulink (f32).
 * Based on CMSIS arm_biquad_cascade_df2T_init_f32.c.
 */

#include <stdint.h>
#include <string.h>

typedef float float32_t;
typedef unsigned int uint32_t;
typedef unsigned char uint8_t;

typedef struct {
    uint8_t numStages;
    float32_t *pState;
    float32_t *pCoeffs;
} arm_biquad_cascade_df2T_instance_f32;

void arm_biquad_cascade_df2T_init_f32(
    arm_biquad_cascade_df2T_instance_f32 * S,
    uint8_t numStages,
    float32_t * pCoeffs,
    float32_t * pState)
{
    S->numStages = numStages;
    S->pCoeffs = pCoeffs;
    memset(pState, 0, (2u * (uint32_t)numStages) * sizeof(float32_t));
    S->pState = pState;
}