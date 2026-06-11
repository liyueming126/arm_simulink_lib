#include <stdint.h>
#include <string.h>

typedef int16_t q15_t;
typedef int32_t arm_status;
#define ARM_MATH_SUCCESS 0
#define ARM_MATH_ARGUMENT_ERROR 1

typedef struct {
    uint16_t numTaps;
    q15_t *pState;
    q15_t *pCoeffs;
} arm_fir_instance_q15;

static arm_status arm_fir_init_q15(
    arm_fir_instance_q15 *S,
    uint16_t numTaps,
    q15_t *pCoeffs,
    q15_t *pState,
    uint32_t blockSize)
{
    if (numTaps & 0x1u)
    {
        return ARM_MATH_ARGUMENT_ERROR;
    }

    S->numTaps = numTaps;
    S->pCoeffs = pCoeffs;
    memset(pState, 0, (numTaps + blockSize) * sizeof(q15_t));
    S->pState = pState;

    return ARM_MATH_SUCCESS;
}
