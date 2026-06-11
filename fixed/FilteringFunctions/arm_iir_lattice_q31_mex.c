#include "mex.h"
#include <stdint.h>
#include <string.h>

typedef int32_t q31_t; typedef int64_t q63_t;
typedef struct { uint16_t numStages; q31_t *pState; q31_t *pkCoeffs; q31_t *pvCoeffs; } arm_iir_lattice_instance_q31;

void arm_iir_lattice_q31(const arm_iir_lattice_instance_q31 *S, q31_t *pSrc, q31_t *pDst, uint32_t blockSize) {
    q31_t fcurr, fnext = 0, gcurr = 0, gnext; q63_t acc; uint32_t blkCnt, tapCnt; q31_t *px1, *px2, *pk, *pv, *pState, *pStateCurnt; uint32_t numStages = S->numStages;
    blkCnt = blockSize; pState = &S->pState[0];
    while (blkCnt > 0u) { fcurr = *pSrc++; px1 = pState; px2 = pState; acc = 0; pv = &S->pvCoeffs[0]; pk = &S->pkCoeffs[0]; tapCnt = numStages;
        while (tapCnt > 0u) { gcurr = *px1++; { int64_t v = (int64_t)fcurr - (int64_t)(((q63_t)gcurr * (*pk)) >> 31); if (v > 2147483647LL) v = 2147483647LL; if (v < -2147483648LL) v = -2147483648LL; fnext = (q31_t)v; } { int64_t v = (int64_t)gcurr + (int64_t)(((q63_t)fnext * (*pk++)) >> 31); if (v > 2147483647LL) v = 2147483647LL; if (v < -2147483648LL) v = -2147483648LL; gnext = (q31_t)v; } acc += ((q63_t)gnext * *pv++); *px2++ = gnext; fcurr = fnext; tapCnt--; }
        acc += (q63_t)fnext * (*pv++); *px2++ = fnext; *pDst++ = (q31_t)(acc >> 31u); pState = pState + 1u; blkCnt--; }
    pStateCurnt = &S->pState[0]; pState = &S->pState[blockSize]; tapCnt = numStages; while (tapCnt > 0u) { *pStateCurnt++ = *pState++; tapCnt--; }
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
    arm_iir_lattice_instance_q31 S; q31_t *pSrc, *pDst; uint32_t blockSize; int32_t i; double *pPk, *pPv, *pSrcIn;
    if (nrhs < 5) mexErrMsgTxt("Need: numStages, pkCoeffs, pvCoeffs, src, blockSize");
    S.numStages = (uint16_t)(mxGetScalar(prhs[0]));
    pPk = mxGetPr(prhs[1]); S.pkCoeffs = (q31_t *)mxMalloc(S.numStages * sizeof(q31_t));
    for (i = 0; i < S.numStages; i++) { double v = pPk[i] * 2147483648.0; if (v > 2147483647.0) v = 2147483647.0; if (v < -2147483648.0) v = -2147483648.0; S.pkCoeffs[i] = (q31_t)((int32_t)v); }
    pPv = mxGetPr(prhs[2]); S.pvCoeffs = (q31_t *)mxMalloc((S.numStages + 1) * sizeof(q31_t));
    for (i = 0; i < S.numStages + 1; i++) { double v = pPv[i] * 2147483648.0; if (v > 2147483647.0) v = 2147483647.0; if (v < -2147483648.0) v = -2147483648.0; S.pvCoeffs[i] = (q31_t)((int32_t)v); }
    blockSize = (uint32_t)(mxGetScalar(prhs[4])); S.pState = (q31_t *)mxMalloc((S.numStages + blockSize) * sizeof(q31_t)); memset(S.pState, 0, (S.numStages + blockSize) * sizeof(q31_t));
    pSrc = (q31_t *)mxMalloc(blockSize * sizeof(q31_t)); pSrcIn = mxGetPr(prhs[3]);
    for (i = 0; i < blockSize; i++) { double v = pSrcIn[i] * 2147483648.0; if (v > 2147483647.0) v = 2147483647.0; if (v < -2147483648.0) v = -2147483648.0; pSrc[i] = (q31_t)((int32_t)v); }
    pDst = (q31_t *)mxMalloc(blockSize * sizeof(q31_t));
    arm_iir_lattice_q31(&S, pSrc, pDst, blockSize);
    plhs[0] = mxCreateDoubleMatrix((int)blockSize, 1, mxREAL);
    { double *out = mxGetPr(plhs[0]); for (i = 0; i < blockSize; i++) out[i] = pDst[i] / 2147483648.0; }
    mxFree(S.pkCoeffs); mxFree(S.pvCoeffs); mxFree(S.pState); mxFree(pSrc); mxFree(pDst);
}