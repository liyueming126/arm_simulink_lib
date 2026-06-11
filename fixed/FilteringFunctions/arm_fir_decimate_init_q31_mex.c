#include "mex.h"
#include <stdint.h>
#include <string.h>

typedef int32_t q31_t;
typedef struct { uint8_t M; uint16_t numTaps; q31_t *pCoeffs; q31_t *pState; } arm_fir_decimate_instance_q31;
typedef enum { ARM_MATH_SUCCESS = 0, ARM_MATH_LENGTH_ERROR = 1 } arm_status;

arm_status arm_fir_decimate_init_q31(
    arm_fir_decimate_instance_q31 *S, uint16_t numTaps, uint8_t M,
    q31_t *pCoeffs, q31_t *pState, uint32_t blockSize)
{
    arm_status status;
    if ((blockSize % M) != 0u) { status = ARM_MATH_LENGTH_ERROR; }
    else { S->numTaps = numTaps; S->pCoeffs = pCoeffs; memset(pState, 0, (numTaps + (blockSize - 1)) * sizeof(q31_t)); S->pState = pState; S->M = M; status = ARM_MATH_SUCCESS; }
    return status;
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
    arm_fir_decimate_instance_q31 S; uint16_t numTaps; uint8_t M; uint32_t blockSize; int32_t i; double *pCoefIn;
    if (nrhs < 4) mexErrMsgTxt("Need: numTaps, M, coeffs, blockSize");
    numTaps = (uint16_t)(mxGetScalar(prhs[0])); M = (uint8_t)(mxGetScalar(prhs[1]));
    pCoefIn = mxGetPr(prhs[2]); q31_t *pCoeffs = (q31_t *)mxMalloc(numTaps * sizeof(q31_t));
    for (i = 0; i < numTaps; i++) { double v = pCoefIn[i] * 2147483648.0; if (v > 2147483647.0) v = 2147483647.0; if (v < -2147483648.0) v = -2147483648.0; pCoeffs[i] = (q31_t)((int32_t)v); }
    blockSize = (uint32_t)(mxGetScalar(prhs[3]));
    q31_t *pState = (q31_t *)mxMalloc((numTaps + blockSize - 1) * sizeof(q31_t));
    arm_status status = arm_fir_decimate_init_q31(&S, numTaps, M, pCoeffs, pState, blockSize);
    plhs[0] = mxCreateDoubleScalar((double)S.numTaps); plhs[1] = mxCreateDoubleScalar((double)S.M); plhs[2] = mxCreateDoubleScalar((double)status);
    mxFree(pCoeffs); mxFree(pState);
}