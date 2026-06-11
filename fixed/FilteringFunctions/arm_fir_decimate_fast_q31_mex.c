#include "mex.h"
#include <stdint.h>
#include <string.h>

typedef int32_t q31_t; typedef int64_t q63_t;
typedef struct { uint8_t M; uint16_t numTaps; q31_t *pCoeffs; q31_t *pState; } arm_fir_decimate_instance_q31;

void arm_fir_decimate_fast_q31(arm_fir_decimate_instance_q31 *S, q31_t *pSrc, q31_t *pDst, uint32_t blockSize) {
    q31_t *pState = S->pState, *pCoeffs = S->pCoeffs, *pStateCurnt, x0, c0, *px, *pb, sum0;
    uint32_t numTaps = S->numTaps, i, tapCnt, blkCnt, outBlockSize = blockSize / S->M;
    pStateCurnt = S->pState + (numTaps - 1u); blkCnt = outBlockSize;
    while (blkCnt > 0u) { i = S->M; do { *pStateCurnt++ = *pSrc++; } while (--i);
        sum0 = 0; px = pState; pb = pCoeffs; tapCnt = numTaps;
        while (tapCnt > 0u) { c0 = *pb++; x0 = *px++; sum0 = (q31_t)((((q63_t)sum0 << 32) + ((q63_t)x0 * c0)) >> 32); tapCnt--; }
        pState = pState + S->M; *pDst++ = (q31_t)(sum0 << 1); blkCnt--; }
    pStateCurnt = S->pState; i = numTaps - 1u; while (i > 0u) { *pStateCurnt++ = *pState++; i--; }
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
    arm_fir_decimate_instance_q31 S; q31_t *pSrc, *pDst; uint32_t blockSize; int32_t i; double *pCoef, *pSrcIn;
    if (nrhs < 5) mexErrMsgTxt("Need: M, numTaps, coeffs, src, blockSize");
    S.M = (uint8_t)(mxGetScalar(prhs[0])); S.numTaps = (uint16_t)(mxGetScalar(prhs[1]));
    pCoef = mxGetPr(prhs[2]); S.pCoeffs = (q31_t *)mxMalloc(S.numTaps * sizeof(q31_t));
    for (i = 0; i < S.numTaps; i++) { double v = pCoef[i] * 2147483648.0; if (v > 2147483647.0) v = 2147483647.0; if (v < -2147483648.0) v = -2147483648.0; S.pCoeffs[i] = (q31_t)((int32_t)v); }
    blockSize = (uint32_t)(mxGetScalar(prhs[4])); S.pState = (q31_t *)mxMalloc((S.numTaps + blockSize - 1) * sizeof(q31_t)); memset(S.pState, 0, (S.numTaps + blockSize - 1) * sizeof(q31_t));
    pSrc = (q31_t *)mxMalloc(blockSize * sizeof(q31_t)); pSrcIn = mxGetPr(prhs[3]);
    for (i = 0; i < blockSize; i++) { double v = pSrcIn[i] * 2147483648.0; if (v > 2147483647.0) v = 2147483647.0; if (v < -2147483648.0) v = -2147483648.0; pSrc[i] = (q31_t)((int32_t)v); }
    uint32_t outBlockSize = blockSize / S.M; pDst = (q31_t *)mxMalloc(outBlockSize * sizeof(q31_t));
    arm_fir_decimate_fast_q31(&S, pSrc, pDst, blockSize);
    plhs[0] = mxCreateDoubleMatrix((int)outBlockSize, 1, mxREAL);
    { double *out = mxGetPr(plhs[0]); for (i = 0; i < outBlockSize; i++) out[i] = pDst[i] / 2147483648.0; }
    mxFree(S.pCoeffs); mxFree(S.pState); mxFree(pSrc); mxFree(pDst);
}