/* ----------------------------------------------------------------------
 * arm_cfft_radix4_init_f32_mex.c
 * MEX wrapper for arm_cfft_radix4_init_f32.
 * Usage: S = arm_cfft_radix4_init_f32_mex(fftLen, ifftFlag, bitReverseFlag)
 * -------------------------------------------------------------------- */
#include <stdint.h>
#include <stdlib.h>
#include "mex.h"

#include "arm_cfft_radix4_init_f32_simulink.c"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    uint16_t fftLen;
    uint8_t ifftFlag, bitReverseFlag;
    arm_cfft_radix4_instance_f32 S;
    int status;
    const char *fieldNames[] = {"fftLen", "ifftFlag", "bitReverseFlag",
                                "twidCoefModifier", "bitRevFactor", "onebyfftLen"};

    if (nrhs != 3)
        mexErrMsgIdAndTxt("arm_cfft_radix4_init_f32_mex:nrhs",
            "Usage: S = arm_cfft_radix4_init_f32_mex(fftLen, ifftFlag, bitReverseFlag)");
    if (nlhs > 1)
        mexErrMsgIdAndTxt("arm_cfft_radix4_init_f32_mex:nlhs", "One output required.");

    fftLen = (uint16_t)mxGetScalar(prhs[0]);
    ifftFlag = (uint8_t)mxGetScalar(prhs[1]);
    bitReverseFlag = (uint8_t)mxGetScalar(prhs[2]);

    status = arm_cfft_radix4_init_f32(&S, fftLen, ifftFlag, bitReverseFlag);
    if (status != 0)
        mexErrMsgIdAndTxt("arm_cfft_radix4_init_f32_mex:len",
            "Invalid FFT length. Supported: 16,64,256,1024,4096.");

    plhs[0] = mxCreateStructMatrix(1, 1, 6, fieldNames);
    mxSetField(plhs[0], 0, "fftLen", mxCreateDoubleScalar((double)S.fftLen));
    mxSetField(plhs[0], 0, "ifftFlag", mxCreateDoubleScalar((double)S.ifftFlag));
    mxSetField(plhs[0], 0, "bitReverseFlag", mxCreateDoubleScalar((double)S.bitReverseFlag));
    mxSetField(plhs[0], 0, "twidCoefModifier", mxCreateDoubleScalar((double)S.twidCoefModifier));
    mxSetField(plhs[0], 0, "bitRevFactor", mxCreateDoubleScalar((double)S.bitRevFactor));
    mxSetField(plhs[0], 0, "onebyfftLen", mxCreateDoubleScalar((double)S.onebyfftLen));

    free(S.pTwiddle);
    free(S.pBitRevTable);
}