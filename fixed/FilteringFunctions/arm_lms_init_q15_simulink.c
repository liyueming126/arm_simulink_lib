#include <stdint.h>
#include <string.h>

typedef int16_t q15_t;
typedef int32_t q31_t;
typedef int64_t q63_t;

typedef struct {
    uint16_t numTaps;
    q15_t *pState;
    q15_t *pCoeffs;
    q15_t mu;
    uint32_t postShift;
} arm_lms_instance_q15;

static void arm_lms_init_q15(
    arm_lms_instance_q15 *S, uint16_t numTaps, q15_t *pCoeffs, q15_t *pState,
    q15_t mu, uint32_t blockSize, uint32_t postShift)
{
    S->numTaps = numTaps;
    S->pCoeffs = pCoeffs;
    memset(pState, 0, (numTaps + (blockSize - 1u)) * sizeof(q15_t));
    S->pState = pState;
    S->mu = mu;
    S->postShift = postShift;
}
