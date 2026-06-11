#include "mex.h"
#include <stdint.h>
#include <string.h>

typedef int16_t q15_t;
typedef int32_t q31_t;
typedef int64_t q63_t;

typedef struct {
    uint8_t M;
    uint16_t numTaps;
    q15_t *pCoeffs;
    q15_t *pState;
} arm_fir_decimate_instance_q15;

static q15_t ssat16(int32_t val) {
    if (val > 32767) return 32767;
    if (val < -32768) return -32768;
    return (q15_t)val;
}

void arm_fir_decimate_q15(
    const arm_fir_decimate_instance_q15 *S,
    q15_t *pSrc,
    q15_t *pDst,
    uint32_t blockSize)
{
    q15_t *pState = S->pState;
    q15_t *pCoeffs = S->pCoeffs;
    q15_t *pStateCurnt;
    q15_t *px;
    q15_t *pb;
    q31_t x0, c0;
    q63_t sum0;
    uint32_t numTaps = S->numTaps;
    uint32_t i, blkCnt, tapCnt, outBlockSize = blockSize / S->M;

    pStateCurnt = S->pState + (numTaps - 1u);
    blkCnt = outBlockSize;

    while (blkCnt > 0u) {
        i = S->M;
        do { *pStateCurnt++ = *pSrc++; } while (--i);
        sum0 = 0; px = pState; pb = pCoeffs; tapCnt = numTaps;
        while (tapCnt > 0u) { c0 = *pb++; x0 = *px++; sum0 += (q31_t)x0 * c0; tapCnt--; }
        pState = pState + S->M;
        *pDst++ = (q15_t)(ssat16((sum0 >> 15)));
        blkCnt--;
    }
    pStateCurnt = S->pState; i = numTaps - 1u;
    while (i > 0u) { *pStateCurnt++ = *pState++; i--; }
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
    arm_fir_decimate_instance_q15 S;
    q15_t *pSrc, *pDst;
    uint32_t blockSize;
    int32_t i;
    double *pCoef, *pSrcIn;

    if (nrhs < 5) mexErrMsgTxt("Need: M, numTaps, coeffs, src, blockSize");
    S.M = (uint8_t)(mxGetScalar(prhs[0]));
    S.numTaps = (uint16_t)(mxGetScalar(prhs[1]));
    pCoef = mxGetPr(prhs[2]);
    S.pCoeffs = (q15_t *)mxMalloc(S.numTaps * sizeof(q15_t));
    for (i = 0; i < S.numTaps; i++) {
        double v = pCoef[i] * 32768.0;
        if (v > 32767.0) v = 32767.0; if (v < -32768.0) v = -32768.0;
        S.pCoeffs[i] = (q15_t)((int16_t)v);
    }
    blockSize = (uint32_t)(mxGetScalar(prhs[4]));
    S.pState = (q15_t *)mxMalloc((S.numTaps + blockSize - 1) * sizeof(q15_t));
    memset(S.pState, 0, (S.numTaps + blockSize - 1) * sizeof(q15_t));
    pSrc = (q15_t *)mxMalloc(blockSize * sizeof(q15_t));
    pSrcIn = mxGetPr(prhs[3]);
    for (i = 0; i < blockSize; i++) {
        double v = pSrcIn[i] * 32768.0;
        if (v > 32767.0) v = 32767.0; if (v < -32768.0) v = -32768.0;
        pSrc[i] = (q15_t)((int16_t)v);
    }
    uint32_t outBlockSize = blockSize / S.M;
    pDst = (q15_t *)mxMalloc(outBlockSize * sizeof(q15_t));
    arm_fir_decimate_q15(&S, pSrc, pDst, blockSize);
    plhs[0] = mxCreateDoubleMatrix((int)outBlockSize, 1, mxREAL);
    { double *out = mxGetPr(plhs[0]); for (i = 0; i < outBlockSize; i++) out[i] = pDst[i] / 32768.0; }
    mxFree(S.pCoeffs); mxFree(S.pState); mxFree(pSrc); mxFree(pDst);
}