#include "mex.h"
#include <stdint.h>
#include <string.h>

typedef int32_t q31_t;
typedef int64_t q63_t;

typedef struct {
    uint32_t numStages;
    q63_t *pState;
    q31_t *pCoeffs;
    uint8_t postShift;
} arm_biquad_cas_df1_32x64_ins_q31;

static q63_t mult32x64(q63_t x, q31_t y) {
    return (q63_t)((int64_t)(x >> 32) * (int64_t)y) + (q63_t)(((int64_t)((uint32_t)x) * (int64_t)y) >> 32);
}

void arm_biquad_cas_df1_32x64_q31(
    const arm_biquad_cas_df1_32x64_ins_q31 *S,
    q31_t *pSrc,
    q31_t *pDst,
    uint32_t blockSize)
{
    q31_t *pIn = pSrc;
    q31_t *pOut = pDst;
    q63_t *pState = S->pState;
    q31_t *pCoeffs = S->pCoeffs;
    q63_t acc;
    q31_t Xn1, Xn2;
    q63_t Yn1, Yn2;
    q31_t b0, b1, b2, a1, a2;
    q31_t Xn;
    int32_t shift = (int32_t)S->postShift + 1;
    uint32_t sample, stage = S->numStages;
    q31_t acc_l, acc_h;
    uint32_t uShift = ((uint32_t)S->postShift + 1u);
    uint32_t lShift = 32u - uShift;

    do {
        b0 = *pCoeffs++; b1 = *pCoeffs++; b2 = *pCoeffs++; a1 = *pCoeffs++; a2 = *pCoeffs++;
        Xn1 = (q31_t)(pState[0]); Xn2 = (q31_t)(pState[1]); Yn1 = pState[2]; Yn2 = pState[3];
        sample = blockSize;

        while (sample > 0u) {
            Xn = *pIn++;
            acc = (q63_t)Xn * b0; acc += (q63_t)Xn1 * b1; acc += (q63_t)Xn2 * b2;
            acc += mult32x64(Yn1, a1); acc += mult32x64(Yn2, a2);
            Xn2 = Xn1; Xn1 = Xn; Yn2 = Yn1; Yn1 = acc << shift;
            acc_l = acc & 0xffffffff; acc_h = (acc >> 32) & 0xffffffff;
            acc_h = (uint32_t)acc_l >> lShift | acc_h << uShift;
            *pOut++ = acc_h; sample--;
        }
        pIn = pDst; pOut = pDst;
        *pState++ = (q63_t)Xn1; *pState++ = (q63_t)Xn2; *pState++ = Yn1; *pState++ = Yn2;
    } while (--stage);
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
    arm_biquad_cas_df1_32x64_ins_q31 S;
    q31_t *pSrc, *pDst;
    uint32_t blockSize;
    int32_t i;
    double *pCoef, *pSrcIn;

    if (nrhs < 5) mexErrMsgTxt("Need: numStages, coeffs, src, blockSize, postShift");
    S.numStages = (uint32_t)(mxGetScalar(prhs[0]));
    S.postShift = (uint8_t)(mxGetScalar(prhs[4]));
    pCoef = mxGetPr(prhs[1]);
    S.pCoeffs = (q31_t *)mxMalloc(5 * S.numStages * sizeof(q31_t));
    for (i = 0; i < 5 * S.numStages; i++) {
        double v = pCoef[i] * 2147483648.0;
        if (v > 2147483647.0) v = 2147483647.0;
        if (v < -2147483648.0) v = -2147483648.0;
        S.pCoeffs[i] = (q31_t)((int32_t)v);
    }
    S.pState = (q63_t *)mxMalloc(4 * S.numStages * sizeof(q63_t));
    memset(S.pState, 0, 4 * S.numStages * sizeof(q63_t));
    blockSize = (uint32_t)(mxGetScalar(prhs[3]));
    pSrc = (q31_t *)mxMalloc(blockSize * sizeof(q31_t));
    pSrcIn = mxGetPr(prhs[2]);
    for (i = 0; i < blockSize; i++) {
        double v = pSrcIn[i] * 2147483648.0;
        if (v > 2147483647.0) v = 2147483647.0;
        if (v < -2147483648.0) v = -2147483648.0;
        pSrc[i] = (q31_t)((int32_t)v);
    }
    pDst = (q31_t *)mxMalloc(blockSize * sizeof(q31_t));
    arm_biquad_cas_df1_32x64_q31(&S, pSrc, pDst, blockSize);
    plhs[0] = mxCreateDoubleMatrix((int)blockSize, 1, mxREAL);
    { double *out = mxGetPr(plhs[0]); for (i = 0; i < blockSize; i++) out[i] = pDst[i] / 2147483648.0; }
    mxFree(S.pCoeffs); mxFree(S.pState); mxFree(pSrc); mxFree(pDst);
}