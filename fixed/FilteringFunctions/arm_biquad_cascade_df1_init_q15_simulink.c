
#include <stdint.h>
#include <string.h>

typedef int16_t q15_t;

typedef struct {
    uint32_t numStages;
    q15_t *pState;
    q15_t *pCoeffs;
    int8_t postShift;
} arm_biquad_casd_df1_inst_q15;

void arm_biquad_cascade_df1_init_q15(
    arm_biquad_casd_df1_inst_q15 *S,
    uint8_t numStages,
    q15_t *pCoeffs,
    q15_t *pState,
    int8_t postShift)
{
    S->numStages = numStages;
    S->postShift = postShift;
    S->pCoeffs = pCoeffs;
    memset(pState, 0, (4u * (uint32_t)numStages) * sizeof(q15_t));
    S->pState = pState;
}
