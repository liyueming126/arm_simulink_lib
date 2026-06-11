#include "mex.h"
#include <stdint.h>
#include <string.h>

typedef int16_t q15_t;
typedef int32_t q31_t;
typedef int64_t q63_t;

typedef struct {
    uint32_t numStages;
    q15_t *pState;
    q15_t *pCoeffs;
    int8_t postShift;
} arm_biquad_casd_df1_inst_q15;

static q15_t ssat16(int32_t val) {
    if (val > 32767) return 32767;
    if (val < -32768) return -32768;
    return (q15_t)val;
}

void arm_biquad_cascade_df1_q15(
    const arm_biquad_casd_df1_inst_q15 *S,
    q15_t *pSrc,
    q15_t *pDst,
    uint32_t blockSize)
{
    q15_t *pIn = pSrc;
    q15_t *pOut = pDst;
    q15_t b0, b1, b2, a1, a2;
    q15_t Xn1, Xn2, Yn1, Yn2;
    q15_t Xn;
    q63_t acc;
    int32_t shift = (15 - (int32_t)S->postShift);
    q15_t *pState = S->pState;
    q15_t *pCoeffs = S->pCoeffs;
    uint32_t sample, stage = (uint32_t)S->numStages;

    do {
        b0 = *pCoeffs++;
        pCoeffs++;
        b1 = *pCoeffs++;
        b2 = *pCoeffs++;
        a1 = *pCoeffs++;
        a2 = *pCoeffs++;

        Xn1 = pState[0];
        Xn2 = pState[1];
        Yn1 = pState[2];
        Yn2 = pState[3];

        sample = blockSize;

        while (sample > 0u) {
            Xn = *pIn++;
            acc = (q31_t)b0 * Xn;
            acc += (q31_t)b1 * Xn1;
            acc += (q31_t)b2 * Xn2;
            acc += (q31_t)a1 * Yn1;
            acc += (q31_t)a2 * Yn2;

            acc = ssat16((acc >> shift));

            Xn2 = Xn1;
            Xn1 = Xn;
            Yn2 = Yn1;
            Yn1 = (q15_t)acc;

            *pOut++ = (q15_t)acc;
            sample--;
        }

        pIn = pDst;
        pOut = pDst;

        *pState++ = Xn1;
        *pState++ = Xn2;
        *pState++ = Yn1;
        *pState++ = Yn2;

    } while (--stage);
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
    arm_biquad_casd_df1_inst_q15 S;
    q15_t *pSrc, *pDst;
    uint32_t blockSize;
    int32_t i;
    double *pCoef, *pStateIn, *pSrcIn;

    if (nrhs < 5) mexErrMsgTxt("Need: state, coeffs, src, blockSize, postShift");

    S.numStages = (uint32_t)(mxGetScalar(prhs[0]));
    S.postShift = (int8_t)(mxGetScalar(prhs[4]));

    pCoef = mxGetPr(prhs[1]);
    S.pCoeffs = (q15_t *)mxMalloc(6 * S.numStages * sizeof(q15_t));
    for (i = 0; i < (int32_t)(6 * S.numStages); i++) {
        double v = pCoef[i] * 32768.0;
        if (v > 32767.0) v = 32767.0;
        if (v < -32768.0) v = -32768.0;
        ((q15_t *)S.pCoeffs)[i] = (q15_t)((int16_t)v);
    }

    S.pState = (q15_t *)mxMalloc(4 * S.numStages * sizeof(q15_t));
    memset(S.pState, 0, 4 * S.numStages * sizeof(q15_t));

    pStateIn = mxGetPr(prhs[0]); /* first arg: state initial values as double, only shape matters */
    blockSize = (uint32_t)(mxGetScalar(prhs[3]));

    pSrc = (q15_t *)mxMalloc(blockSize * sizeof(q15_t));
    pSrcIn = mxGetPr(prhs[2]);
    for (i = 0; i < blockSize; i++) {
        double v = pSrcIn[i] * 32768.0;
        if (v > 32767.0) v = 32767.0;
        if (v < -32768.0) v = -32768.0;
        pSrc[i] = (q15_t)((int16_t)v);
    }

    pDst = (q15_t *)mxMalloc(blockSize * sizeof(q15_t));

    arm_biquad_cascade_df1_q15(&S, pSrc, pDst, blockSize);

    plhs[0] = mxCreateDoubleMatrix((int)blockSize, 1, mxREAL);
    {
        double *out = mxGetPr(plhs[0]);
        for (i = 0; i < blockSize; i++) out[i] = pDst[i] / 32768.0;
    }

    mxFree(S.pCoeffs);
    mxFree(S.pState);
    mxFree(pSrc);
    mxFree(pDst);
}