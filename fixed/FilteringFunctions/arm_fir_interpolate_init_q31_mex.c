#include "mex.h"
#include <stdint.h>
#include <string.h>

typedef int32_t q31_t;
typedef struct { uint8_t L; uint16_t phaseLength; q31_t *pCoeffs; q31_t *pState; } arm_fir_interpolate_instance_q31;
typedef enum { ARM_MATH_SUCCESS = 0, ARM_MATH_LENGTH_ERROR = 1 } arm_status;

arm_status arm_fir_interpolate_init_q31(
    arm_fir_interpolate_instance_q31 *S, uint8_t L, uint16_t numTaps,
    q31_t *pCoeffs, q31_t *pState, uint32_t blockSize)
{ arm_status status; if ((numTaps % L) != 0u) { status = ARM_MATH_LENGTH_ERROR; } else { S->pCoeffs = pCoeffs; S->L = L; S->phaseLength = numTaps / L; memset(pState, 0, (blockSize + ((uint32_t)S->phaseLength - 1u)) * sizeof(q31_t)); S->pState = pState; status = ARM_MATH_SUCCESS; } return status; }

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
    arm_fir_interpolate_instance_q31 S; uint8_t L; uint16_t numTaps; uint32_t blockSize; int32_t i; double *pCoefIn;
    if (nrhs < 4) mexErrMsgTxt("Need: L, numTaps, coeffs, blockSize");
    L = (uint8_t)(mxGetScalar(prhs[0])); numTaps = (uint16_t)(mxGetScalar(prhs[1]));
    pCoefIn = mxGetPr(prhs[2]); q31_t *pCoeffs = (q31_t *)mxMalloc(numTaps * sizeof(q31_t));
    for (i = 0; i < numTaps; i++) { double v = pCoefIn[i] * 2147483648.0; if (v > 2147483647.0) v = 2147483647.0; if (v < -2147483648.0) v = -2147483648.0; pCoeffs[i] = (q31_t)((int32_t)v); }
    blockSize = (uint32_t)(mxGetScalar(prhs[3]));
    uint16_t phaseLen = numTaps / L;
    q31_t *pState = (q31_t *)mxMalloc((blockSize + phaseLen - 1) * sizeof(q31_t));
    arm_status status = arm_fir_interpolate_init_q31(&S, L, numTaps, pCoeffs, pState, blockSize);
    plhs[0] = mxCreateDoubleScalar((double)S.L); plhs[1] = mxCreateDoubleScalar((double)S.phaseLength); plhs[2] = mxCreateDoubleScalar((double)status);
    mxFree(pCoeffs); mxFree(pState);
}