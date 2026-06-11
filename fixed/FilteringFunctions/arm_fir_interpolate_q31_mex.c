#include "mex.h"
#include <stdint.h>
#include <string.h>

typedef int32_t q31_t; typedef int64_t q63_t;
typedef struct { uint8_t L; uint16_t phaseLength; q31_t *pCoeffs; q31_t *pState; } arm_fir_interpolate_instance_q31;

void arm_fir_interpolate_q31(const arm_fir_interpolate_instance_q31 *S, q31_t *pSrc, q31_t *pDst, uint32_t blockSize) {
    q31_t *pState = S->pState, *pCoeffs = S->pCoeffs, *pStateCurnt, *ptr1, *ptr2;
    q63_t sum; q31_t x0, c0; uint32_t i, blkCnt; uint16_t phaseLen = S->phaseLength, tapCnt;
    pStateCurnt = S->pState + ((q31_t)phaseLen - 1); blkCnt = blockSize;
    while (blkCnt > 0u) { *pStateCurnt++ = *pSrc++; i = S->L;
        while (i > 0u) { sum = 0; ptr1 = pState; ptr2 = pCoeffs + (i - 1u); tapCnt = phaseLen;
            while (tapCnt > 0u) { c0 = *(ptr2); ptr2 += S->L; x0 = *ptr1++; sum += (q63_t)x0 * c0; tapCnt--; }
            *pDst++ = (q31_t)(sum >> 31); i--; }
        pState = pState + 1; blkCnt--; }
    pStateCurnt = S->pState; tapCnt = phaseLen - 1u; while (tapCnt > 0u) { *pStateCurnt++ = *pState++; tapCnt--; }
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
    arm_fir_interpolate_instance_q31 S; q31_t *pSrc, *pDst; uint32_t blockSize; int32_t i; double *pCoef, *pSrcIn;
    if (nrhs < 5) mexErrMsgTxt("Need: L, numTaps, coeffs, src, blockSize");
    S.L = (uint8_t)(mxGetScalar(prhs[0])); uint16_t numTaps = (uint16_t)(mxGetScalar(prhs[1]));
    S.phaseLength = numTaps / S.L; pCoef = mxGetPr(prhs[2]); S.pCoeffs = (q31_t *)mxMalloc(numTaps * sizeof(q31_t));
    for (i = 0; i < numTaps; i++) { double v = pCoef[i] * 2147483648.0; if (v > 2147483647.0) v = 2147483647.0; if (v < -2147483648.0) v = -2147483648.0; S.pCoeffs[i] = (q31_t)((int32_t)v); }
    blockSize = (uint32_t)(mxGetScalar(prhs[4])); S.pState = (q31_t *)mxMalloc((blockSize + S.phaseLength - 1) * sizeof(q31_t)); memset(S.pState, 0, (blockSize + S.phaseLength - 1) * sizeof(q31_t));
    pSrc = (q31_t *)mxMalloc(blockSize * sizeof(q31_t)); pSrcIn = mxGetPr(prhs[3]);
    for (i = 0; i < blockSize; i++) { double v = pSrcIn[i] * 2147483648.0; if (v > 2147483647.0) v = 2147483647.0; if (v < -2147483648.0) v = -2147483648.0; pSrc[i] = (q31_t)((int32_t)v); }
    uint32_t outBlockSize = blockSize * S.L; pDst = (q31_t *)mxMalloc(outBlockSize * sizeof(q31_t));
    arm_fir_interpolate_q31(&S, pSrc, pDst, blockSize);
    plhs[0] = mxCreateDoubleMatrix((int)outBlockSize, 1, mxREAL);
    { double *out = mxGetPr(plhs[0]); for (i = 0; i < outBlockSize; i++) out[i] = pDst[i] / 2147483648.0; }
    mxFree(S.pCoeffs); mxFree(S.pState); mxFree(pSrc); mxFree(pDst);
}