#include "mex.h"
#include <stdint.h>
#include <math.h>

#include "arm_cfft_radix2_q15_simulink.c"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    q15_t *pData;
    uint16_t fftLen;
    uint8_t ifftFlag, bitReverseFlag;
    uint32_t n;
    mwSize i;

    if (nrhs < 3) mexErrMsgTxt("Usage: y = arm_cfft_radix2_q15_mex(data, ifftFlag, bitReverseFlag)");
    if (!mxIsInt16(prhs[0]) || mxIsComplex(prhs[0]) || mxIsSparse(prhs[0]))
        mexErrMsgTxt("data must be real int16.");

    fftLen = (uint16_t)(mxGetNumberOfElements(prhs[0]) / 2);
    ifftFlag = (uint8_t)mxGetScalar(prhs[1]);
    bitReverseFlag = (uint8_t)mxGetScalar(prhs[2]);

    /* Validate FFT length */
    if (fftLen < 16 || fftLen > 4096 || (fftLen & (fftLen - 1)) != 0)
        mexErrMsgTxt("fftLen must be a power of 2 between 16 and 4096.");

    /* Allocate output */
    n = 2 * fftLen;
    plhs[0] = mxCreateNumericMatrix(1, n, mxINT16_CLASS, mxREAL);
    pData = (q15_t *)mxGetData(prhs[0]);
    for (i = 0; i < (mwSize)n; i++) ((q15_t *)mxGetData(plhs[0]))[i] = pData[i];
    pData = (q15_t *)mxGetData(plhs[0]);

    /* Generate twiddle table: full 4096-entry table (CMSIS convention for radix-2) */
    {
        uint32_t j;
        double pi = 3.14159265358979323846;
        q15_t *twiddle = (q15_t *)mxCalloc(4096 * 2, sizeof(q15_t));
        if (!twiddle) mexErrMsgTxt("Failed to allocate twiddle table.");
        for (j = 0; j < 4096; j++) {
            double angle = 2.0 * pi * (double)j / 4096.0;
            twiddle[2u * j]     = (q15_t)((int32_t)(cos(angle) * 32768.0 + (cos(angle) >= 0 ? 0.5 : -0.5)));
            twiddle[2u * j + 1u] = (q15_t)((int32_t)(sin(angle) * 32768.0 + (sin(angle) >= 0 ? 0.5 : -0.5)));
        }

        /* Run the radix-2 butterfly directly */
        uint16_t twidCoefModifier = 4096 / fftLen;
        if (ifftFlag)
            arm_radix2_butterfly_inverse_q15(pData, fftLen, twiddle, twidCoefModifier);
        else
            arm_radix2_butterfly_q15(pData, fftLen, twiddle, twidCoefModifier);

        mxFree(twiddle);
    }

    /* Inline bit-reversal for radix-2 */
    if (bitReverseFlag) {
        uint32_t i, j = 0;
        for (i = 0; i < (uint32_t)fftLen - 1; i++) {
            if (i < j) {
                q15_t tr = pData[2 * i];
                q15_t ti = pData[2 * i + 1];
                pData[2 * i] = pData[2 * j];
                pData[2 * i + 1] = pData[2 * j + 1];
                pData[2 * j] = tr;
                pData[2 * j + 1] = ti;
            }
            uint32_t k = fftLen >> 1;
            while (k <= j) { j -= k; k >>= 1; }
            j += k;
        }
    }
}