#include "mex.h"
#include <stdint.h>
#include <string.h>

typedef int16_t q15_t;
typedef int32_t q31_t;

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

void arm_biquad_cascade_df1_fast_q15(
    const arm_biquad_casd_df1_inst_q15 *S,
    q15_t *pSrc,
    q15_t *pDst,
    uint32_t blockSize)
{
    q15_t *pIn = pSrc;
    q15_t *pOut = pDst;
    q31_t acc;
    int32_t shift = (int32_t)(15 - S->postShift);
    q15_t *pState = S->pState;
    q15_t *pCoeffs = S->pCoeffs;
    uint32_t sample, stage = S->numStages;

    do {
        q15_t b0, b1, b2, a1, a2;
        q15_t x_n1, x_n2, y_n1, y_n2;

        b0 = *pCoeffs++;
        b1 = *pCoeffs++;
        b2 = *pCoeffs++;
        a1 = *pCoeffs++;
        a2 = *pCoeffs++;
        pCoeffs++; /* skip padding */

        x_n1 = pState[0];
        x_n2 = pState[1];
        y_n1 = pState[2];
        y_n2 = pState[3];
        pState += 4;

        sample = blockSize;

        while (sample > 0u) {
            q15_t in = *pIn++;

            /* acc = b0*in + b1*x_n1 + b2*x_n2 - a1*y_n1 - a2*y_n2 */
            acc  = (q31_t)b0 * in;
            acc += (q31_t)b1 * x_n1;
            acc += (q31_t)b2 * x_n2;
            acc -= (q31_t)a1 * y_n1;
            acc -= (q31_t)a2 * y_n2;

            q15_t out = ssat16(acc >> shift);
            *pOut++ = out;

            /* Update state shift registers */
            x_n2 = x_n1;
            x_n1 = in;
            y_n2 = y_n1;
            y_n1 = out;

            sample--;
        }

        pIn = pDst;
        pOut = pDst;

        pState -= 4;
        pState[0] = x_n1;
        pState[1] = x_n2;
        pState[2] = y_n1;
        pState[3] = y_n2;
        pState += 4;

        stage--;
    } while (stage > 0u);
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
    arm_biquad_casd_df1_inst_q15 S;
    q15_t *pSrc, *pDst;
    uint32_t blockSize;
    int32_t i;
    double *pCoef, *pSrcIn;

    if (nrhs < 5) mexErrMsgTxt("Need: numStages, coeffs, src, blockSize, postShift");

    S.numStages = (uint32_t)(mxGetScalar(prhs[0]));
    S.postShift = (int8_t)(mxGetScalar(prhs[4]));
    pCoef = mxGetPr(prhs[1]);
    S.pCoeffs = (q15_t *)mxMalloc(6 * S.numStages * sizeof(q15_t));
    for (i = 0; i < 6 * S.numStages; i++) {
        double v = pCoef[i] * 32768.0;
        if (v > 32767.0) v = 32767.0;
        if (v < -32768.0) v = -32768.0;
        S.pCoeffs[i] = (q15_t)((int16_t)v);
    }

    S.pState = (q15_t *)mxMalloc(4 * S.numStages * sizeof(q15_t));
    memset(S.pState, 0, 4 * S.numStages * sizeof(q15_t));
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
    arm_biquad_cascade_df1_fast_q15(&S, pSrc, pDst, blockSize);

    plhs[0] = mxCreateDoubleMatrix((int)blockSize, 1, mxREAL);
    { double *out = mxGetPr(plhs[0]); for (i = 0; i < blockSize; i++) out[i] = pDst[i] / 32768.0; }

    mxFree(S.pCoeffs); mxFree(S.pState); mxFree(pSrc); mxFree(pDst);
}