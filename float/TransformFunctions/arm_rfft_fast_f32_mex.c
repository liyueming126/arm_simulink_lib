/* ----------------------------------------------------------------------
 * arm_rfft_fast_f32_mex.c
 * MEX wrapper for arm_rfft_fast_f32 (fast real FFT).
 * Usage: y = arm_rfft_fast_f32_mex(x, fftLen, ifftFlag)
 *   x: single real input vector of length fftLen
 *   fftLen: 32, 64, 128, 256, 512, 1024, 2048, 4096
 *   ifftFlag: 0=forward RFFT, 1=inverse RIFFT
 * The output is packed as [X(0), X(N/2), X(1), X(2), ..., X(N/2-1)]
 * where each X(k) is complex interleaved (real,imag).
 * -------------------------------------------------------------------- */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "mex.h"

#include "arm_rfft_fast_f32_simulink.c"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    float *pSrc, *pData, *pOut;
    size_t dataLen;
    uint16_t fftLen;
    uint8_t ifftFlag;
    arm_rfft_fast_instance_f32 S;

    if (nrhs < 2 || nrhs > 3)
        mexErrMsgIdAndTxt("arm_rfft_fast_f32_mex:nrhs", "Usage: y = arm_rfft_fast_f32_mex(x, fftLen, ifftFlag)");
    if (nlhs > 1)
        mexErrMsgIdAndTxt("arm_rfft_fast_f32_mex:nlhs", "One output allowed.");
    if (!mxIsSingle(prhs[0]) || mxIsComplex(prhs[0]))
        mexErrMsgIdAndTxt("arm_rfft_fast_f32_mex:type", "Input must be real single.");

    dataLen = mxGetNumberOfElements(prhs[0]);
    fftLen = (uint16_t)mxGetScalar(prhs[1]);
    if (dataLen != fftLen)
        mexErrMsgIdAndTxt("arm_rfft_fast_f32_mex:len", "Input length must equal fftLen.");
    ifftFlag = (nrhs >= 3) ? (uint8_t)(mxGetScalar(prhs[2]) != 0) : 0;

    /* Validate length */
    if (fftLen != 32 && fftLen != 64 && fftLen != 128 && fftLen != 256 &&
        fftLen != 512 && fftLen != 1024 && fftLen != 2048 && fftLen != 4096)
        mexErrMsgIdAndTxt("arm_rfft_fast_f32_mex:support", "Supported: 32,64,128,256,512,1024,2048,4096.");

    /* Initialize instance */
    S.fftLenRFFT = fftLen;
    S.Sint.fftLen = fftLen / 2;

    /* Generate CFFT twiddle */
    {
        uint32_t i, len = (3u * S.Sint.fftLen) / 4u;
        float *tw = (float *)mxMalloc(len * 2 * sizeof(float));
        for (i = 0; i < len; i++) {
            double angle = -2.0 * 3.14159265358979323846 * (double)i / (double)S.Sint.fftLen;
            tw[2*i] = (float)cos(angle); tw[2*i+1] = (float)sin(angle);
        }
        S.Sint.pTwiddle = tw;
    }

    /* Generate RFFT twiddle */
    {
        uint32_t i, n = fftLen / 2;
        float *tw = (float *)mxMalloc(2 * n * sizeof(float));
        for (i = 0; i < n; i++) {
            double angle = -3.14159265358979323846 * (double)i / (double)n;
            tw[2*i] = (float)cos(angle); tw[2*i+1] = (float)sin(angle);
        }
        S.pTwiddleRFFT = tw;
    }

    /* Generate bit reversal table */
    {
        uint32_t i, bits = 0, temp = S.Sint.fftLen;
        while (temp > 1) { temp >>= 1; bits++; }
        uint32_t tlen = 2u * S.Sint.fftLen;
        uint16_t *tbl = (uint16_t *)mxMalloc(tlen * sizeof(uint16_t));
        uint32_t ti = 0;
        for (i = 0; i < S.Sint.fftLen; i++) {
            uint32_t rev = 0, idx = i, b;
            for (b = 0; b < bits; b++) { rev = (rev << 1) | (idx & 1); idx >>= 1; }
            if (rev > i) {
                tbl[ti++] = (uint16_t)(2u * i * sizeof(float));
                tbl[ti++] = (uint16_t)(2u * rev * sizeof(float));
            }
        }
        S.Sint.pBitRevTable = tbl;
        S.Sint.bitRevLength = 2u * S.Sint.fftLen;
    }

    /* For forward: p is input (real), pOut is output (complex packed).
       For inverse: p is input (complex packed), pOut is output (real). */
    pSrc = (float *)mxGetData(prhs[0]);

    if (ifftFlag) {
        /* Inverse: input is complex packed (N/2 complex = N elements), output is real */
        plhs[0] = mxCreateNumericMatrix(1, (mwSize)fftLen, mxSINGLE_CLASS, mxREAL);
        pData = (float *)mxMalloc(fftLen * sizeof(float));  /* work buffer */
        memcpy(pData, pSrc, fftLen * sizeof(float));
        pOut = (float *)mxGetData(plhs[0]);
        arm_rfft_fast_f32(&S, pData, pOut, 1);
        mxFree(pData);
    } else {
        /* Forward: input is real (N), output is complex packed (N/2 complex = N elements) */
        plhs[0] = mxCreateNumericMatrix(1, (mwSize)fftLen, mxSINGLE_CLASS, mxREAL);
        pData = (float *)mxMalloc(2 * fftLen * sizeof(float)); /* interleaved CFFT input */
        memset(pData, 0, 2 * fftLen * sizeof(float));
        memcpy(pData, pSrc, fftLen * sizeof(float));
        pOut = (float *)mxGetData(plhs[0]);
        arm_rfft_fast_f32(&S, pData, pOut, 0);
        mxFree(pData);
    }

    mxFree((void *)S.Sint.pTwiddle);
    mxFree(S.pTwiddleRFFT);
    mxFree((void *)S.Sint.pBitRevTable);
}