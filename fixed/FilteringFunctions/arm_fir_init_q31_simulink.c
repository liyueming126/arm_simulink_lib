#include <stdint.h>
#include <string.h>

typedef int32_t q31_t;

typedef struct {
    uint16_t numTaps;
    q31_t *pState;
    q31_t *pCoeffs;
} arm_fir_instance_q31;

static void arm_fir_init_q31(
    arm_fir_instance_q31 *S,
    uint16_t numTaps,
    q31_t *pCoeffs,
    q31_t *pState,
    uint32_t blockSize)
{
    S->numTaps = numTaps;
    S->pCoeffs = pCoeffs;
    memset(pState, 0, (blockSize + ((uint32_t)numTaps - 1u)) * sizeof(q31_t));
    S->pState = pState;
}
