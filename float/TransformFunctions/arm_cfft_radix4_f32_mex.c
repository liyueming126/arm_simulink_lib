/* ----------------------------------------------------------------------
 * arm_cfft_radix4_f32_mex.c
 * MEX wrapper for arm_cfft_radix4_f32.
 * Usage: y = arm_cfft_radix4_f32_mex(x, ifftFlag)
 * -------------------------------------------------------------------- */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "mex.h"

#include "arm_cfft_radix4_f32_simulink.c"

/* Generate twiddle factors: exp(-j*2*pi*k/N) */
static float *gen_twiddle_r4(uint16_t n)
{
    uint32_t i;
    float *table = (float *)malloc(n * 2u * sizeof(float));
    if (!table) return 0;
    for (i = 0; i < n; i++) {
        double angle = -2.0 * 3.14159265358979323846 * (double)i / (double)n;
        table[2u * i]     = (float)cos(angle);
        table[2u * i + 1u] = (float)sin(angle);
    }
    return table;
}

/* Generate bit reversal table */
static uint16_t *gen_bitrev_r4(uint16_t n)
{
    uint32_t i, bits = 0, temp = n;
    uint16_t *table;
    while (temp > 1) { temp >>= 1; bits++; }
    table = (uint16_t *)malloc((n - 4) * sizeof(uint16_t));
    if (!table) return 0;
    for (i = 0; i < n - 4; i++) {
        uint32_t rev = 0, idx = i + 1, b;
        for (b = 0; b < bits; b++) { rev = (rev << 1) | (idx & 1); idx >>= 1; }
        table[i] = (uint16_t)(rev >> 1);
    }
    return table;
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    float *pSrc, *pData;
    size_t dataLen;
    uint16_t fftLen;
    uint8_t ifftFlag;
    arm_cfft_radix4_instance_f32 S;

    if (nrhs < 1 || nrhs > 2)
        mexErrMsgIdAndTxt("arm_cfft_radix4_f32_mex:nrhs", "Usage: y = arm_cfft_radix4_f32_mex(x, ifftFlag)");
    if (nlhs > 1)
        mexErrMsgIdAndTxt("arm_cfft_radix4_f32_mex:nlhs", "One output allowed.");
    if (!mxIsSingle(prhs[0]) || mxIsComplex(prhs[0]))
        mexErrMsgIdAndTxt("arm_cfft_radix4_f32_mex:type", "Input must be real single.");

    dataLen = mxGetNumberOfElements(prhs[0]);
    if (dataLen % 2 != 0)
        mexErrMsgIdAndTxt("arm_cfft_radix4_f32_mex:even", "Input length must be even.");
    fftLen = (uint16_t)(dataLen / 2);

    ifftFlag = (nrhs >= 2) ? (uint8_t)(mxGetScalar(prhs[1]) != 0) : 0;

    /* Supported lengths: 16, 64, 256, 1024, 4096 */
    switch (fftLen) {
        case 16: case 64: case 256: case 1024: case 4096: break;
        default: mexErrMsgIdAndTxt("arm_cfft_radix4_f32_mex:len",
            "Supported: 16,64,256,1024,4096");
    }

    S.fftLen = fftLen;
    S.ifftFlag = ifftFlag;
    S.bitReverseFlag = 1;
    S.pTwiddle = gen_twiddle_r4(fftLen);
    S.pBitRevTable = gen_bitrev_r4(fftLen);

    switch (fftLen) {
        case 4096: S.twidCoefModifier = 1u;   S.bitRevFactor = 1u;   S.onebyfftLen = 0.000244140625f;   break;
        case 1024: S.twidCoefModifier = 4u;   S.bitRevFactor = 4u;   S.onebyfftLen = 0.0009765625f;    break;
        case 256:  S.twidCoefModifier = 16u;  S.bitRevFactor = 16u;  S.onebyfftLen = 0.00390625f;      break;
        case 64:   S.twidCoefModifier = 64u;  S.bitRevFactor = 64u;  S.onebyfftLen = 0.015625f;        break;
        case 16:   S.twidCoefModifier = 256u; S.bitRevFactor = 256u; S.onebyfftLen = 0.0625f;          break;
    }

    plhs[0] = mxCreateNumericMatrix(1, (mwSize)dataLen, mxSINGLE_CLASS, mxREAL);
    pSrc = (float *)mxGetData(prhs[0]);
    pData = (float *)mxGetData(plhs[0]);
    memcpy(pData, pSrc, dataLen * sizeof(float));

    arm_cfft_radix4_f32(&S, pData);

    free(S.pTwiddle);
    free(S.pBitRevTable);
}