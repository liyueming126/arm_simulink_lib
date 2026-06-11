
#include <stdint.h>
#include <string.h>

typedef int32_t q31_t;
typedef int64_t q63_t;

typedef struct {
    uint32_t numStages;
    q63_t *pState;
    q31_t *pCoeffs;
    uint8_t postShift;
} arm_biquad_cas_df1_32x64_ins_q31;

void arm_biquad_cas_df1_32x64_init_q31(
    arm_biquad_cas_df1_32x64_ins_q31 *S,
    uint8_t numStages,
    q31_t *pCoeffs,
    q63_t *pState,
    uint8_t postShift)
{
    S->numStages = numStages;
    S->postShift = postShift;
    S->pCoeffs = pCoeffs;
    memset(pState, 0, (4u * (uint32_t)numStages) * sizeof(q63_t));
    S->pState = pState;
}
