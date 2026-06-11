/* Simplified implementation of ARM FIR sparse init for Simulink.
 * Based on CMSIS arm_fir_sparse_init_f32.c.
 */

#include <stdint.h>
#include <string.h>

typedef float float32_t;
typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef signed int int32_t;

typedef struct {
    uint16_t numTaps;
    int32_t stateIndex;
    float32_t *pState;
    float32_t *pCoeffs;
    uint16_t maxDelay;
    int32_t *pTapDelay;
} arm_fir_sparse_instance_f32;

void arm_fir_sparse_init_f32(
    arm_fir_sparse_instance_f32 * S,
    uint16_t numTaps,
    float32_t * pCoeffs,
    float32_t * pState,
    int32_t * pTapDelay,
    uint16_t maxDelay,
    uint32_t blockSize)
{
    S->numTaps = numTaps;
    S->pCoeffs = pCoeffs;
    S->pTapDelay = pTapDelay;
    S->maxDelay = maxDelay;
    S->stateIndex = 0u;
    memset(pState, 0, (maxDelay + blockSize) * sizeof(float32_t));
    S->pState = pState;
}