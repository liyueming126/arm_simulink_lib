#include "mex.h"
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

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
    arm_biquad_casd_df1_inst_q31 S;
    uint8_t numStages;
    q31_t *pState;
    int8_t postShift = 0;
    double *pCoeffsIn;
    int32_t i;

    if (nrhs < 2) mexErrMsgTxt("Need: numStages, coeffs");
    numStages = (uint8_t)(mxGetScalar(prhs[0]));
    q31_t *pCoeffs = (q31_t *)mxMalloc(5 * numStages * sizeof(q31_t));
    pCoeffsIn = mxGetPr(prhs[1]);
    for (i = 0; i < 5 * numStages; i++) {
        double v = pCoeffsIn[i] * 2147483648.0;
        if (v > 2147483647.0) v = 2147483647.0;
        if (v < -2147483648.0) v = -2147483648.0;
        pCoeffs[i] = (q31_t)((int32_t)v);
    }
    pState = (q31_t *)mxMalloc(4 * numStages * sizeof(q31_t));
    arm_biquad_cascade_df1_init_q31(&S, numStages, pCoeffs, pState, postShift);
    plhs[0] = mxCreateDoubleScalar((double)S.numStages);
    plhs[1] = mxCreateDoubleScalar((double)S.postShift);
    mxFree(pCoeffs); mxFree(pState);
}