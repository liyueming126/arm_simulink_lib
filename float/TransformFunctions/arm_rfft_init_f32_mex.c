/* ----------------------------------------------------------------------
 * arm_rfft_init_f32_mex.c
 * MEX wrapper for arm_rfft_init_f32.
 * Usage: S = arm_rfft_init_f32_mex(fftLen, ifftFlag, bitReverseFlag)
 * -------------------------------------------------------------------- */
#include <stdint.h>
#include <stdlib.h>
#include "mex.h"

#include "arm_rfft_init_f32_simulink.c"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    uint16_t fftLen;
    uint8_t ifftFlag, bitReverseFlag;
    arm_rfft_instance_f32 S;
    int status;

    if (nrhs != 3)
        mexErrMsgIdAndTxt("arm_rfft_init_f32_mex:nrhs",
            "Usage: S = arm_rfft_init_f32_mex(fftLen, ifftFlag, bitReverseFlag)");
    if (nlhs > 1)
        mexErrMsgIdAndTxt("arm_rfft_init_f32_mex:nlhs", "One output required.");

    fftLen = (uint16_t)mxGetScalar(prhs[0]);
    ifftFlag = (uint8_t)mxGetScalar(prhs[1]);
    bitReverseFlag = (uint8_t)mxGetScalar(prhs[2]);

    status = arm_rfft_init_f32(&S, fftLen, ifftFlag, bitReverseFlag);
    if (status != 0)
        mexErrMsgIdAndTxt("arm_rfft_init_f32_mex:len",
            "Unsupported FFT length. Supported: 32,64,128,256,512,1024,2048,4096.");

    plhs[0] = mxCreateStructMatrix(1, 1, 5, (const char *[]){"fftLenReal","fftLenBy2","ifftFlagR","bitReverseFlagR","twidCoefRModifier"});
    mxSetField(plhs[0], 0, "fftLenReal", mxCreateDoubleScalar((double)S.fftLenReal));
    mxSetField(plhs[0], 0, "fftLenBy2", mxCreateDoubleScalar((double)S.fftLenBy2));
    mxSetField(plhs[0], 0, "ifftFlagR", mxCreateDoubleScalar((double)S.ifftFlagR));
    mxSetField(plhs[0], 0, "bitReverseFlagR", mxCreateDoubleScalar((double)S.bitReverseFlagR));
    mxSetField(plhs[0], 0, "twidCoefRModifier", mxCreateDoubleScalar((double)S.twidCoefRModifier));

    arm_rfft_init_f32_free(&S);
}