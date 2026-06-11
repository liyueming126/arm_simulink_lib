/* Simplified implementation of ARM Biquad DF2T init for Simulink (f64).
 * Based on CMSIS arm_biquad_cascade_df2T_init_f64.c.
 */

#include <stdint.h>
#include <string.h>

typedef double float64_t;
typedef unsigned int uint32_t;
typedef unsigned char uint8_t;

typedef struct {
    uint8_t numStages;
    float64_t *pState;
    float64_t *pCoeffs;
} arm_biquad_cascade_df2T_instance_f64;

void arm_biquad_cascade_df2T_init_f64(
    arm_biquad_cascade_df2T_instance_f64 * S,
    uint8_t numStages,
    float64_t * pCoeffs,
    float64_t * pState)
{
    S->numStages = numStages;
    S->pCoeffs = pCoeffs;
    memset(pState, 0, (2u * (uint32_t)numStages) * sizeof(float64_t));
    S->pState = pState;
}