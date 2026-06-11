/* ----------------------------------------------------------------------
 * arm_cfft_radix8_f32_mex.c
 * MEX wrapper for arm_radix8_butterfly_f32.
 * Usage: y = arm_cfft_radix8_f32_mex(x, fftLen, twidCoefModifier)
 *   x: single vector (interleaved complex), must be length 2*fftLen
 *   fftLen: must be 64, 512, or 4096
 *   twidCoefModifier: typically 1
 * -------------------------------------------------------------------- */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "mex.h"

#include "arm_cfft_radix8_f32_simulink.c"

/* Generate twiddle table for radix-8: exp(-j*2*pi*k/N) for k=0..3*N/4-1 */
static float *gen_twiddle_r8(uint16_t n)
{
    uint32_t i;
    uint32_t len = (3u * n) / 4u;
    float *table = (float *)malloc(len * 2u * sizeof(float));
    if (!table) return 0;
    for (i = 0; i < len; i++) {
        double angle = -2.0 * 3.14159265358979323846 * (double)i / (double)n;
        table[2u * i]     = (float)cos(angle);
        table[2u * i + 1u] = (float)sin(angle);
    }
    return table;
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    float *pSrc, *pData;
    size_t dataLen;
    uint16_t fftLen, twidCoefModifier;
    float *pTwiddle;

    if (nrhs != 3)
        mexErrMsgIdAndTxt("arm_cfft_radix8_f32_mex:nrhs",
            "Usage: y = arm_cfft_radix8_f32_mex(x, fftLen, twidCoefModifier)");
    if (nlhs > 1)
        mexErrMsgIdAndTxt("arm_cfft_radix8_f32_mex:nlhs", "One output allowed.");
    if (!mxIsSingle(prhs[0]) || mxIsComplex(prhs[0]))
        mexErrMsgIdAndTxt("arm_cfft_radix8_f32_mex:type", "Input must be real single.");

    dataLen = mxGetNumberOfElements(prhs[0]);
    if (dataLen % 2 != 0)
        mexErrMsgIdAndTxt("arm_cfft_radix8_f32_mex:even", "Input length must be even.");
    fftLen = (uint16_t)(dataLen / 2);

    if (fftLen != 64 && fftLen != 512 && fftLen != 4096)
        mexErrMsgIdAndTxt("arm_cfft_radix8_f32_mex:len", "fftLen must be 64, 512, or 4096.");

    twidCoefModifier = (uint16_t)mxGetScalar(prhs[2]);

    pTwiddle = gen_twiddle_r8(fftLen);

    plhs[0] = mxCreateNumericMatrix(1, (mwSize)dataLen, mxSINGLE_CLASS, mxREAL);
    pSrc = (float *)mxGetData(prhs[0]);
    pData = (float *)mxGetData(plhs[0]);
    memcpy(pData, pSrc, dataLen * sizeof(float));

    arm_radix8_butterfly_f32(pData, fftLen, pTwiddle, twidCoefModifier);

    free(pTwiddle);
}