/* ----------------------------------------------------------------------
 * arm_dct4_f32_mex.c
 * MEX wrapper for arm_dct4_f32 (DCT Type IV).
 * Usage: y = arm_dct4_f32_mex(x, N)
 *   x: single real input vector of length N
 *   N: DCT length (supported: 32, 64, 128, 256, 512, 1024, 2048, 4096)
 * -------------------------------------------------------------------- */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "mex.h"

#include "arm_dct4_f32_simulink.c"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    float *pSrc, *pData, *pState;
    size_t dataLen;
    uint16_t N;
    arm_dct4_instance_f32 S;

    if (nrhs != 2)
        mexErrMsgIdAndTxt("arm_dct4_f32_mex:nrhs", "Usage: y = arm_dct4_f32_mex(x, N)");
    if (nlhs > 1)
        mexErrMsgIdAndTxt("arm_dct4_f32_mex:nlhs", "One output allowed.");
    if (!mxIsSingle(prhs[0]) || mxIsComplex(prhs[0]))
        mexErrMsgIdAndTxt("arm_dct4_f32_mex:type", "Input must be real single.");

    dataLen = mxGetNumberOfElements(prhs[0]);
    N = (uint16_t)mxGetScalar(prhs[1]);
    if (dataLen != N)
        mexErrMsgIdAndTxt("arm_dct4_f32_mex:len", "Input length must equal N.");

    /* Set up DCT4 instance */
    S.N = N;
    S.Nby2 = N / 2;
    S.normalize = (float)sqrt(2.0 / (double)N);

    /* Generate weights and cos factor tables */
    {
        uint32_t i;
        S.pTwiddle = (float *)mxMalloc(2 * N * sizeof(float));
        S.pCosFactor = (float *)mxMalloc(N * sizeof(float));
        for (i = 0; i < N; i++) {
            double a = 3.14159265358979323846 * (double)i / (double)(2u * N);
            S.pTwiddle[2*i] = (float)cos(a);
            S.pTwiddle[2*i+1] = (float)(-sin(a));
            double b = 3.14159265358979323846 * (double)(2u * i + 1u) / (double)(4u * N);
            S.pCosFactor[i] = (float)cos(b);
        }
    }

    /* Create embedded RFFT instance */
    S.pRfft = create_rfft_tables(N);

    /* Allocate state buffer (2*N for complex RFFT output) */
    pState = (float *)mxMalloc(2 * N * sizeof(float));

    /* Create output (same length as input for DCT4) */
    plhs[0] = mxCreateNumericMatrix(1, (mwSize)N, mxSINGLE_CLASS, mxREAL);
    pSrc = (float *)mxGetData(prhs[0]);
    pData = (float *)mxGetData(plhs[0]);

    /* Copy input to working buffer (lib operates in-place) */
    memcpy(pData, pSrc, N * sizeof(float));

    arm_dct4_f32(&S, pState, pData);

    mxFree(S.pTwiddle);
    mxFree(S.pCosFactor);
    mxFree(pState);
    destroy_rfft_tables((arm_rfft_instance_f32 *)S.pRfft);
}