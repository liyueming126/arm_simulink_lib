#include "mex.h"
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

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
    arm_biquad_casd_df1_inst_q15 S;
    uint8_t numStages;
    q15_t *pState;
    int8_t postShift;
    double *pCoeffsIn;
    int32_t i;

    if (nrhs < 2) mexErrMsgTxt("Need: numStages, coeffs");
    numStages = (uint8_t)(mxGetScalar(prhs[0]));

    q15_t *pCoeffs = (q15_t *)mxMalloc(6 * numStages * sizeof(q15_t));
    pCoeffsIn = mxGetPr(prhs[1]);
    for (i = 0; i < 6 * numStages; i++) {
        double v = pCoeffsIn[i] * 32768.0;
        if (v > 32767.0) v = 32767.0;
        if (v < -32768.0) v = -32768.0;
        pCoeffs[i] = (q15_t)((int16_t)v);
    }

    pState = (q15_t *)mxMalloc(4 * numStages * sizeof(q15_t));
    postShift = 0;
    arm_biquad_cascade_df1_init_q15(&S, numStages, pCoeffs, pState, postShift);

    plhs[0] = mxCreateDoubleScalar((double)S.numStages);
    plhs[1] = mxCreateDoubleScalar((double)S.postShift);

    mxFree(pCoeffs);
    mxFree(pState);
}