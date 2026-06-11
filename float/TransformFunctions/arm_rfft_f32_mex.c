/* ----------------------------------------------------------------------
 * arm_rfft_f32_mex.c
 * MEX wrapper for arm_rfft_f32 (deprecated real FFT).
 * Usage: y = arm_rfft_f32_mex(x, fftLen, ifftFlag)
 *   x: single real input vector of length fftLen
 *   fftLen: 32, 64, 128, 256, 512, 1024, 2048, 4096
 *   ifftFlag: 0=forward RFFT, 1=inverse RIFFT
 * -------------------------------------------------------------------- */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "mex.h"

#include "arm_rfft_f32_simulink.c"

/* Generate tables for the internal CFFT */
static arm_cfft_radix4_instance_f32 *setup_cfft(uint16_t fftLen, uint8_t ifftFlag)
{
    arm_cfft_radix4_instance_f32 *cfft = (arm_cfft_radix4_instance_f32 *)mxCalloc(1, sizeof(arm_cfft_radix4_instance_f32));
    if (!cfft) mexErrMsgIdAndTxt("arm_rfft_f32_mex:alloc", "Failed to allocate CFFT instance.");

    cfft->fftLen = fftLen;
    cfft->ifftFlag = ifftFlag;
    cfft->bitReverseFlag = 1;

    cfft->pTwiddle = (float *)mxMalloc(fftLen * 2 * sizeof(float));
    {
        uint32_t i;
        for (i = 0; i < fftLen; i++) {
            double angle = -2.0 * 3.14159265358979323846 * (double)i / (double)fftLen;
            cfft->pTwiddle[2*i] = (float)cos(angle);
            cfft->pTwiddle[2*i+1] = (float)sin(angle);
        }
    }

    {
        uint32_t i, bits = 0, temp = fftLen, n = fftLen;
        uint16_t *table;
        while (temp > 1) { temp >>= 1; bits++; }
        table = (uint16_t *)mxMalloc((n - 4) * sizeof(uint16_t));
        for (i = 0; i < n - 4; i++) {
            uint32_t rev = 0, idx = i + 1, b;
            for (b = 0; b < bits; b++) { rev = (rev << 1) | (idx & 1); idx >>= 1; }
            table[i] = (uint16_t)(rev >> 1);
        }
        cfft->pBitRevTable = table;
    }

    cfft->twidCoefModifier = 1;
    cfft->bitRevFactor = 1;
    cfft->onebyfftLen = 1.0f / (float)fftLen;
    return cfft;
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    float *pSrc, *pDst;
    size_t dataLen;
    uint16_t fftLen;
    uint8_t ifftFlag;
    arm_rfft_instance_f32 S;
    arm_cfft_radix4_instance_f32 *cfft;

    if (nrhs < 2 || nrhs > 3)
        mexErrMsgIdAndTxt("arm_rfft_f32_mex:nrhs", "Usage: y = arm_rfft_f32_mex(x, fftLen, ifftFlag)");
    if (nlhs > 1)
        mexErrMsgIdAndTxt("arm_rfft_f32_mex:nlhs", "One output allowed.");
    if (!mxIsSingle(prhs[0]) || mxIsComplex(prhs[0]))
        mexErrMsgIdAndTxt("arm_rfft_f32_mex:type", "Input must be real single.");

    dataLen = mxGetNumberOfElements(prhs[0]);
    fftLen = (uint16_t)mxGetScalar(prhs[1]);
    if (dataLen != fftLen)
        mexErrMsgIdAndTxt("arm_rfft_f32_mex:len", "Input length must equal fftLen.");
    ifftFlag = (nrhs >= 3) ? (uint8_t)(mxGetScalar(prhs[2]) != 0) : 0;

    /* Check supported lengths */
    if (fftLen != 32 && fftLen != 64 && fftLen != 128 && fftLen != 256 &&
        fftLen != 512 && fftLen != 1024 && fftLen != 2048 && fftLen != 4096)
        mexErrMsgIdAndTxt("arm_rfft_f32_mex:support", "Supported: 32,64,128,256,512,1024,2048,4096.");

    S.fftLenReal = fftLen;
    S.fftLenBy2 = fftLen / 2;
    S.ifftFlagR = ifftFlag;
    S.bitReverseFlagR = 1;

    /* Generate A and B tables */
    {
        uint32_t i, n = fftLen;
        S.pTwiddleAReal = (float *)mxMalloc(2 * n * sizeof(float));
        S.pTwiddleBReal = (float *)mxMalloc(2 * n * sizeof(float));
        for (i = 0; i < n; i++) {
            double angle = 2.0 * 3.14159265358979323846 * (double)i / (double)(2u * n);
            S.pTwiddleAReal[2*i]   = (float)(0.5 * (1.0 - sin(angle)));
            S.pTwiddleAReal[2*i+1] = (float)(0.5 * (-1.0 * cos(angle)));
            S.pTwiddleBReal[2*i]   = (float)(0.5 * (1.0 + sin(angle)));
            S.pTwiddleBReal[2*i+1] = (float)(0.5 * (1.0 * cos(angle)));
        }
    }

    switch (fftLen) {
        case 4096: S.twidCoefRModifier = 1u; break;
        case 2048: S.twidCoefRModifier = 2u; break;
        case 1024: S.twidCoefRModifier = 4u; break;
        case 512:  S.twidCoefRModifier = 8u; break;
        case 256:  S.twidCoefRModifier = 16u; break;
        case 128:  S.twidCoefRModifier = 32u; break;
        case 64:   S.twidCoefRModifier = 64u; break;
        case 32:   S.twidCoefRModifier = 128u; break;
        default: S.twidCoefRModifier = 1u;
    }

    /* Setup internal CFFT */
    if (ifftFlag)
        cfft = setup_cfft(fftLen / 2, 1);
    else
        cfft = setup_cfft(fftLen / 2, 0);
    S.pCfft = cfft;

    /* Output: RFFT produces 2*fftLen complex output (interleaved) */
    size_t outLen = 2u * fftLen;
    plhs[0] = mxCreateNumericMatrix(1, (mwSize)outLen, mxSINGLE_CLASS, mxREAL);
    pSrc = (float *)mxGetData(prhs[0]);
    pDst = (float *)mxGetData(plhs[0]);

    /* For RFFT: pSrc is real input, pDst is complex output.
       Copy real input to work buffer (need fftLen length for real processing) */
    float *work = (float *)mxCalloc(2 * fftLen, sizeof(float));
    memcpy(work, pSrc, fftLen * sizeof(float));

    arm_rfft_f32(&S, work, pDst);

    mxFree(S.pTwiddleAReal);
    mxFree(S.pTwiddleBReal);
    mxFree(cfft->pTwiddle);
    mxFree(cfft->pBitRevTable);
    mxFree(cfft);
    mxFree(work);
}