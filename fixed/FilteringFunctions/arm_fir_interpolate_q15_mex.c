#include "mex.h"
#include <stdint.h>
#include <string.h>

typedef int16_t q15_t; typedef int32_t q31_t; typedef int64_t q63_t;
typedef struct { uint8_t L; uint16_t phaseLength; q15_t *pCoeffs; q15_t *pState; } arm_fir_interpolate_instance_q15;

static q15_t ssat16(int32_t val) { if (val > 32767) return 32767; if (val < -32768) return -32768; return (q15_t)val; }

void arm_fir_interpolate_q15(const arm_fir_interpolate_instance_q15 *S, q15_t *pSrc, q15_t *pDst, uint32_t blockSize) {
    q15_t *pState = S->pState, *pCoeffs = S->pCoeffs, *pStateCurnt, *ptr1, *ptr2;
    q63_t sum; q15_t x0, c0; uint32_t i, blkCnt, tapCnt; uint16_t phaseLen = S->phaseLength;
    pStateCurnt = S->pState + (phaseLen - 1u); blkCnt = blockSize;
    while (blkCnt > 0u) { *pStateCurnt++ = *pSrc++; i = S->L;
        while (i > 0u) { sum = 0; ptr1 = pState; ptr2 = pCoeffs + (i - 1u); tapCnt = (uint32_t)phaseLen;
            while (tapCnt > 0u) { c0 = *ptr2; ptr2 += S->L; x0 = *ptr1++; sum += ((q31_t)x0 * c0); tapCnt--; }
            *pDst++ = (q15_t)(ssat16((sum >> 15))); i--; }
        pState = pState + 1; blkCnt--; }
    pStateCurnt = S->pState; i = (uint32_t)phaseLen - 1u;
    while (i > 0u) { *pStateCurnt++ = *pState++; i--; }
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
    arm_fir_interpolate_instance_q15 S; q15_t *pSrc, *pDst; uint32_t blockSize; int32_t i; double *pCoef, *pSrcIn;
    if (nrhs < 5) mexErrMsgTxt("Need: L, numTaps, coeffs, src, blockSize");
    S.L = (uint8_t)(mxGetScalar(prhs[0])); uint16_t numTaps = (uint16_t)(mxGetScalar(prhs[1]));
    S.phaseLength = numTaps / S.L;
    pCoef = mxGetPr(prhs[2]); S.pCoeffs = (q15_t *)mxMalloc(numTaps * sizeof(q15_t));
    for (i = 0; i < numTaps; i++) { double v = pCoef[i] * 32768.0; if (v > 32767.0) v = 32767.0; if (v < -32768.0) v = -32768.0; S.pCoeffs[i] = (q15_t)((int16_t)v); }
    blockSize = (uint32_t)(mxGetScalar(prhs[4]));
    S.pState = (q15_t *)mxMalloc((blockSize + S.phaseLength - 1) * sizeof(q15_t)); memset(S.pState, 0, (blockSize + S.phaseLength - 1) * sizeof(q15_t));
    pSrc = (q15_t *)mxMalloc(blockSize * sizeof(q15_t)); pSrcIn = mxGetPr(prhs[3]);
    for (i = 0; i < blockSize; i++) { double v = pSrcIn[i] * 32768.0; if (v > 32767.0) v = 32767.0; if (v < -32768.0) v = -32768.0; pSrc[i] = (q15_t)((int16_t)v); }
    uint32_t outBlockSize = blockSize * S.L;
    pDst = (q15_t *)mxMalloc(outBlockSize * sizeof(q15_t));
    arm_fir_interpolate_q15(&S, pSrc, pDst, blockSize);
    plhs[0] = mxCreateDoubleMatrix((int)outBlockSize, 1, mxREAL);
    { double *out = mxGetPr(plhs[0]); for (i = 0; i < outBlockSize; i++) out[i] = pDst[i] / 32768.0; }
    mxFree(S.pCoeffs); mxFree(S.pState); mxFree(pSrc); mxFree(pDst);
}