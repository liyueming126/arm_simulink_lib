
#include <stdint.h>
#include <string.h>

typedef int16_t q15_t;
typedef struct { uint8_t L; uint16_t phaseLength; q15_t *pCoeffs; q15_t *pState; } arm_fir_interpolate_instance_q15;
typedef enum { ARM_MATH_SUCCESS = 0, ARM_MATH_LENGTH_ERROR = 1 } arm_status;

arm_status arm_fir_interpolate_init_q15(
    arm_fir_interpolate_instance_q15 *S, uint8_t L, uint16_t numTaps,
    q15_t *pCoeffs, q15_t *pState, uint32_t blockSize)
{ arm_status status; if ((numTaps % L) != 0u) { status = ARM_MATH_LENGTH_ERROR; } else { S->pCoeffs = pCoeffs; S->L = L; S->phaseLength = numTaps / L; memset(pState, 0, (blockSize + ((uint32_t)S->phaseLength - 1u)) * sizeof(q15_t)); S->pState = pState; status = ARM_MATH_SUCCESS; } return status; }
