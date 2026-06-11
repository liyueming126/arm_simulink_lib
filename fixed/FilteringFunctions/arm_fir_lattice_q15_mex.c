#include "mex.h"
#include <stdint.h>
#include <string.h>

typedef int16_t q15_t; typedef int32_t q31_t;
typedef struct { uint16_t numStages; q15_t *pState; q15_t *pCoeffs; } arm_fir_lattice_instance_q15;

static q15_t ssat16(int32_t val) { if (val > 32767) return 32767; if (val < -32768) return -32768; return (q15_t)val; }

void arm_fir_lattice_q15(const arm_fir_lattice_instance_q15 *S, q15_t *pSrc, q15_t *pDst, uint32_t blockSize) {
    q31_t fcurnt, fnext, gcurnt, gnext; uint32_t numStages = S->numStages, blkCnt, stageCnt; q15_t *pState, *px, *pk;
    pState = &S->pState[0]; blkCnt = blockSize;
    while (blkCnt > 0u) { fcurnt = *pSrc++; pk = (S->pCoeffs); px = pState; gcurnt = *px;
        fnext = ((gcurnt * (*pk)) >> 15u) + fcurnt; fnext = ssat16(fnext);
        gnext = ((fcurnt * (*pk++)) >> 15u) + gcurnt; gnext = ssat16(gnext);
        *px++ = (q15_t)fcurnt; fcurnt = fnext; stageCnt = (numStages - 1u);
        while (stageCnt > 0u) { gcurnt = *px; *px++ = (q15_t)gnext;
            fnext = ((gcurnt * (*pk)) >> 15u) + fcurnt; fnext = ssat16(fnext);
            gnext = ((fcurnt * (*pk++)) >> 15u) + gcurnt; gnext = ssat16(gnext);
            fcurnt = fnext; stageCnt--; }
        *pDst++ = ssat16(fcurnt); blkCnt--; }
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
    arm_fir_lattice_instance_q15 S; q15_t *pSrc, *pDst; uint32_t blockSize; int32_t i; double *pCoef, *pSrcIn;
    if (nrhs < 4) mexErrMsgTxt("Need: numStages, coeffs, src, blockSize");
    S.numStages = (uint16_t)(mxGetScalar(prhs[0]));
    pCoef = mxGetPr(prhs[1]); S.pCoeffs = (q15_t *)mxMalloc(S.numStages * sizeof(q15_t));
    for (i = 0; i < S.numStages; i++) { double v = pCoef[i] * 32768.0; if (v > 32767.0) v = 32767.0; if (v < -32768.0) v = -32768.0; S.pCoeffs[i] = (q15_t)((int16_t)v); }
    S.pState = (q15_t *)mxMalloc(S.numStages * sizeof(q15_t)); memset(S.pState, 0, S.numStages * sizeof(q15_t));
    blockSize = (uint32_t)(mxGetScalar(prhs[3]));
    pSrc = (q15_t *)mxMalloc(blockSize * sizeof(q15_t)); pSrcIn = mxGetPr(prhs[2]);
    for (i = 0; i < blockSize; i++) { double v = pSrcIn[i] * 32768.0; if (v > 32767.0) v = 32767.0; if (v < -32768.0) v = -32768.0; pSrc[i] = (q15_t)((int16_t)v); }
    pDst = (q15_t *)mxMalloc(blockSize * sizeof(q15_t));
    arm_fir_lattice_q15(&S, pSrc, pDst, blockSize);
    plhs[0] = mxCreateDoubleMatrix((int)blockSize, 1, mxREAL);
    { double *out = mxGetPr(plhs[0]); for (i = 0; i < blockSize; i++) out[i] = pDst[i] / 32768.0; }
    mxFree(S.pCoeffs); mxFree(S.pState); mxFree(pSrc); mxFree(pDst);
}