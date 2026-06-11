
#include <stdint.h>
#include <string.h>

typedef int32_t q31_t;
typedef struct { uint8_t M; uint16_t numTaps; q31_t *pCoeffs; q31_t *pState; } arm_fir_decimate_instance_q31;
typedef enum { ARM_MATH_SUCCESS = 0, ARM_MATH_LENGTH_ERROR = 1 } arm_status;

arm_status arm_fir_decimate_init_q31(
    arm_fir_decimate_instance_q31 *S, uint16_t numTaps, uint8_t M,
    q31_t *pCoeffs, q31_t *pState, uint32_t blockSize)
{
    arm_status status;
    if ((blockSize % M) != 0u) { status = ARM_MATH_LENGTH_ERROR; }
    else { S->numTaps = numTaps; S->pCoeffs = pCoeffs; memset(pState, 0, (numTaps + (blockSize - 1)) * sizeof(q31_t)); S->pState = pState; S->M = M; status = ARM_MATH_SUCCESS; }
    return status;
}
