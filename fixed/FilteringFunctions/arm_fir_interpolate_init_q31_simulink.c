
#include <stdint.h>
#include <string.h>

typedef int32_t q31_t;
typedef struct { uint8_t L; uint16_t phaseLength; q31_t *pCoeffs; q31_t *pState; } arm_fir_interpolate_instance_q31;
typedef enum { ARM_MATH_SUCCESS = 0, ARM_MATH_LENGTH_ERROR = 1 } arm_status;

arm_status arm_fir_interpolate_init_q31(
    arm_fir_interpolate_instance_q31 *S, uint8_t L, uint16_t numTaps,
    q31_t *pCoeffs, q31_t *pState, uint32_t blockSize)
{ arm_status status; if ((numTaps % L) != 0u) { status = ARM_MATH_LENGTH_ERROR; } else { S->pCoeffs = pCoeffs; S->L = L; S->phaseLength = numTaps / L; memset(pState, 0, (blockSize + ((uint32_t)S->phaseLength - 1u)) * sizeof(q31_t)); S->pState = pState; status = ARM_MATH_SUCCESS; } return status; }
