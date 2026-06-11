#include "mex.h"
#include <stdint.h>
#include <string.h>

typedef int16_t q15_t;
typedef struct { uint8_t M; uint16_t numTaps; q15_t *pCoeffs; q15_t *pState; } arm_fir_decimate_instance_q15;
typedef enum { ARM_MATH_SUCCESS = 0, ARM_MATH_LENGTH_ERROR = 1 } arm_status;

arm_status arm_fir_decimate_init_q15(
    arm_fir_decimate_instance_q15 *S, uint16_t numTaps, uint8_t M,
    q15_t *pCoeffs, q15_t *pState, uint32_t blockSize)
{
    arm_status status;
    if ((blockSize % M) != 0u) { status = ARM_MATH_LENGTH_ERROR; }
    else { S->numTaps = numTaps; S->pCoeffs = pCoeffs; memset(pState, 0, (numTaps + (blockSize - 1u)) * sizeof(q15_t)); S->pState = pState; S->M = M; status = ARM_MATH_SUCCESS; }
    return status;
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
    arm_fir_decimate_instance_q15 S; uint16_t numTaps; uint8_t M; uint32_t blockSize; int32_t i; double *pCoefIn;
    if (nrhs < 4) mexErrMsgTxt("Need: numTaps, M, coeffs, blockSize");
    numTaps = (uint16_t)(mxGetScalar(prhs[0])); M = (uint8_t)(mxGetScalar(prhs[1]));
    pCoefIn = mxGetPr(prhs[2]); q15_t *pCoeffs = (q15_t *)mxMalloc(numTaps * sizeof(q15_t));
    for (i = 0; i < numTaps; i++) { double v = pCoefIn[i] * 32768.0; if (v > 32767.0) v = 32767.0; if (v < -32768.0) v = -32768.0; pCoeffs[i] = (q15_t)((int16_t)v); }
    blockSize = (uint32_t)(mxGetScalar(prhs[3]));
    q15_t *pState = (q15_t *)mxMalloc((numTaps + blockSize - 1) * sizeof(q15_t));
    arm_status status = arm_fir_decimate_init_q15(&S, numTaps, M, pCoeffs, pState, blockSize);
    plhs[0] = mxCreateDoubleScalar((double)S.numTaps);
    plhs[1] = mxCreateDoubleScalar((double)S.M);
    plhs[2] = mxCreateDoubleScalar((double)status);
    mxFree(pCoeffs); mxFree(pState);
}