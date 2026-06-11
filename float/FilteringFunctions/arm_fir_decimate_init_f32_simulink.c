/* Simplified implementation of ARM FIR decimate init for Simulink.
 * Based on CMSIS arm_fir_decimate_init_f32.c.
 */

#include <stdint.h>
#include <string.h>

typedef float float32_t;
typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;

typedef struct {
    uint8_t M;
    uint16_t numTaps;
    float32_t *pCoeffs;
    float32_t *pState;
} arm_fir_decimate_instance_f32;

typedef int arm_status;

#ifndef ARM_MATH_SUCCESS
#define ARM_MATH_SUCCESS 0
#endif
#ifndef ARM_MATH_LENGTH_ERROR
#define ARM_MATH_LENGTH_ERROR -1
#endif

arm_status arm_fir_decimate_init_f32(
    arm_fir_decimate_instance_f32 * S,
    uint16_t numTaps,
    uint8_t M,
    float32_t * pCoeffs,
    float32_t * pState,
    uint32_t blockSize)
{
    if ((blockSize % M) != 0u) {
        return ARM_MATH_LENGTH_ERROR;
    }
    S->numTaps = numTaps;
    S->pCoeffs = pCoeffs;
    memset(pState, 0, (numTaps + (blockSize - 1u)) * sizeof(float32_t));
    S->pState = pState;
    S->M = M;
    return ARM_MATH_SUCCESS;
}