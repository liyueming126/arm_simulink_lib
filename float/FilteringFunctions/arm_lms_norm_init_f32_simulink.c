/* Simplified implementation of ARM NLMS init for Simulink.
 * Based on CMSIS arm_lms_norm_init_f32.c.
 */

#include <stdint.h>
#include <string.h>

typedef float float32_t;
typedef unsigned int uint32_t;
typedef unsigned short uint16_t;

typedef struct {
    uint16_t numTaps;
    float32_t *pState;
    float32_t *pCoeffs;
    float32_t mu;
    float32_t energy;
    float32_t x0;
} arm_lms_norm_instance_f32;

void arm_lms_norm_init_f32(
    arm_lms_norm_instance_f32 * S,
    uint16_t numTaps,
    float32_t * pCoeffs,
    float32_t * pState,
    float32_t mu,
    uint32_t blockSize)
{
    S->numTaps = numTaps;
    S->pCoeffs = pCoeffs;
    memset(pState, 0, (numTaps + (blockSize - 1u)) * sizeof(float32_t));
    S->pState = pState;
    S->mu = mu;
    S->energy = 0.0f;
    S->x0 = 0.0f;
}