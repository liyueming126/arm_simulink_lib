#include "mex.h"
#include <stdint.h>

#include "arm_cfft_radix4_q15_simulink.c"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    q15_t *pData;
    uint16_t fftLen;
    uint8_t ifftFlag, bitReverseFlag;
    arm_cfft_radix4_instance_q15 S;
    uint32_t n;
    mwSize i;

    if (nrhs < 3) mexErrMsgTxt("Usage: y = arm_cfft_radix4_q15_mex(data, ifftFlag, bitReverseFlag)");
    if (!mxIsInt16(prhs[0]) || mxIsComplex(prhs[0]) || mxIsSparse(prhs[0]))
        mexErrMsgTxt("data must be real int16.");

    fftLen = (uint16_t)(mxGetNumberOfElements(prhs[0]) / 2);
    ifftFlag = (uint8_t)mxGetScalar(prhs[1]);
    bitReverseFlag = (uint8_t)mxGetScalar(prhs[2]);

    S.fftLen = fftLen;
    S.ifftFlag = ifftFlag;
    S.bitReverseFlag = bitReverseFlag;
    S.pTwiddle = NULL;
    S.pBitRevTable = NULL;
    switch (fftLen) {
        case 4096u: S.twidCoefModifier = 1u;   S.bitRevFactor = 1u;   break;
        case 2048u: S.twidCoefModifier = 2u;   S.bitRevFactor = 2u;   break;
        case 1024u: S.twidCoefModifier = 4u;   S.bitRevFactor = 4u;   break;
        case  512u: S.twidCoefModifier = 8u;   S.bitRevFactor = 8u;   break;
        case  256u: S.twidCoefModifier = 16u;  S.bitRevFactor = 16u;  break;
        case  128u: S.twidCoefModifier = 32u;  S.bitRevFactor = 32u;  break;
        case   64u: S.twidCoefModifier = 64u;  S.bitRevFactor = 64u;  break;
        case   32u: S.twidCoefModifier = 128u; S.bitRevFactor = 128u; break;
        case   16u: S.twidCoefModifier = 256u; S.bitRevFactor = 256u; break;
        default: mexErrMsgTxt("Unsupported fftLen.");
    }

    n = 2 * fftLen;
    plhs[0] = mxCreateNumericMatrix(1, n, mxINT16_CLASS, mxREAL);
    pData = (q15_t *)mxGetData(prhs[0]);
    for (i = 0; i < (mwSize)n; i++) ((q15_t *)mxGetData(plhs[0]))[i] = pData[i];

    arm_cfft_radix4_q15(&S, (q15_t *)mxGetData(plhs[0]));
}