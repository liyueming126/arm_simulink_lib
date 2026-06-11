
#include <stdint.h>
#include <string.h>

typedef int32_t q31_t;

typedef struct {
    uint32_t numStages;
    q31_t *pState;
    q31_t *pCoeffs;
    uint8_t postShift;
} arm_biquad_casd_df1_inst_q31;

void arm_biquad_cascade_df1_init_q31(
    arm_biquad_casd_df1_inst_q31 *S,
    uint8_t numStages,
    q31_t *pCoeffs,
    q31_t *pState,
    int8_t postShift)
{
    S->numStages = numStages;
    S->postShift = postShift;
    S->pCoeffs = pCoeffs;
    memset(pState, 0, (4u * (uint32_t)numStages) * sizeof(q31_t));
    S->pState = pState;
}
