/* Simplified implementation of ARM FIR interpolate init for Simulink.
 * Based on CMSIS arm_fir_interpolate_init_f32.c.
 */

#include <stdint.h>
#include <string.h>

typedef float float32_t;
typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;

typedef struct {
    uint8_t L;
    uint16_t phaseLength;
    float32_t *pCoeffs;
    float32_t *pState;
} arm_fir_interpolate_instance_f32;

typedef int arm_status;

#ifndef ARM_MATH_SUCCESS
#define ARM_MATH_SUCCESS 0
#endif
#ifndef ARM_MATH_LENGTH_ERROR
#define ARM_MATH_LENGTH_ERROR -1
#endif

arm_status arm_fir_interpolate_init_f32(
    arm_fir_interpolate_instance_f32 * S,
    uint8_t L,
    uint16_t numTaps,
    float32_t * pCoeffs,
    float32_t * pState,
    uint32_t blockSize)
{
    if ((numTaps % L) != 0u) {
        return ARM_MATH_LENGTH_ERROR;
    }
    S->pCoeffs = pCoeffs;
    S->L = L;
    S->phaseLength = numTaps / L;
    memset(pState, 0, (blockSize + ((uint32_t)S->phaseLength - 1u)) * sizeof(float32_t));
    S->pState = pState;
    return ARM_MATH_SUCCESS;
}