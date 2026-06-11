#include <stdint.h>
#include <string.h>

typedef int8_t q7_t;

typedef struct {
    uint16_t numTaps;
    q7_t *pState;
    q7_t *pCoeffs;
} arm_fir_instance_q7;

static void arm_fir_init_q7(
    arm_fir_instance_q7 *S,
    uint16_t numTaps,
    q7_t *pCoeffs,
    q7_t *pState,
    uint32_t blockSize)
{
    S->numTaps = numTaps;
    S->pCoeffs = pCoeffs;
    memset(pState, 0, (numTaps + (blockSize - 1u)) * sizeof(q7_t));
    S->pState = pState;
}
