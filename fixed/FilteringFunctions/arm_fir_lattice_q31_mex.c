#include "mex.h"
#include <stdint.h>
#include <string.h>

typedef int32_t q31_t; typedef int64_t q63_t;
typedef struct { uint16_t numStages; q31_t *pState; q31_t *pCoeffs; } arm_fir_lattice_instance_q31;

void arm_fir_lattice_q31(const arm_fir_lattice_instance_q31 *S, q31_t *pSrc, q31_t *pDst, uint32_t blockSize) {
    q31_t *pState, *px, *pk, fcurr, fnext, gcurr, gnext; uint32_t numStages = S->numStages, blkCnt, stageCnt;
    pState = &S->pState[0]; blkCnt = blockSize;
    while (blkCnt > 0u) { fcurr = *pSrc++; pk = (S->pCoeffs); px = pState; gcurr = *px;
        fnext = (q31_t)(((q63_t)gcurr * (*pk)) >> 31) + fcurr; gnext = (q31_t)(((q63_t)fcurr * (*pk++)) >> 31) + gcurr;
        *px++ = fcurr; fcurr = fnext; stageCnt = (numStages - 1u);
        while (stageCnt > 0u) { gcurr = *px; *px++ = gnext; fnext = (q31_t)(((q63_t)gcurr * (*pk)) >> 31) + fcurr; gnext = (q31_t)(((q63_t)fcurr * (*pk++)) >> 31) + gcurr; fcurr = fnext; stageCnt--; }
        *pDst++ = fcurr; blkCnt--; }
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
    arm_fir_lattice_instance_q31 S; q31_t *pSrc, *pDst; uint32_t blockSize; int32_t i; double *pCoef, *pSrcIn;
    if (nrhs < 4) mexErrMsgTxt("Need: numStages, coeffs, src, blockSize");
    S.numStages = (uint16_t)(mxGetScalar(prhs[0]));
    pCoef = mxGetPr(prhs[1]); S.pCoeffs = (q31_t *)mxMalloc(S.numStages * sizeof(q31_t));
    for (i = 0; i < S.numStages; i++) { double v = pCoef[i] * 2147483648.0; if (v > 2147483647.0) v = 2147483647.0; if (v < -2147483648.0) v = -2147483648.0; S.pCoeffs[i] = (q31_t)((int32_t)v); }
    S.pState = (q31_t *)mxMalloc(S.numStages * sizeof(q31_t)); memset(S.pState, 0, S.numStages * sizeof(q31_t));
    blockSize = (uint32_t)(mxGetScalar(prhs[3])); pSrc = (q31_t *)mxMalloc(blockSize * sizeof(q31_t)); pSrcIn = mxGetPr(prhs[2]);
    for (i = 0; i < blockSize; i++) { double v = pSrcIn[i] * 2147483648.0; if (v > 2147483647.0) v = 2147483647.0; if (v < -2147483648.0) v = -2147483648.0; pSrc[i] = (q31_t)((int32_t)v); }
    pDst = (q31_t *)mxMalloc(blockSize * sizeof(q31_t));
    arm_fir_lattice_q31(&S, pSrc, pDst, blockSize);
    plhs[0] = mxCreateDoubleMatrix((int)blockSize, 1, mxREAL);
    { double *out = mxGetPr(plhs[0]); for (i = 0; i < blockSize; i++) out[i] = pDst[i] / 2147483648.0; }
    mxFree(S.pCoeffs); mxFree(S.pState); mxFree(pSrc); mxFree(pDst);
}