/* ----------------------------------------------------------------------
 * arm_rfft_fast_init_f32_mex.c
 * MEX wrapper for arm_rfft_fast_init_f32.
 * Usage: S = arm_rfft_fast_init_f32_mex(fftLen)
 * -------------------------------------------------------------------- */
#include <stdint.h>
#include <stdlib.h>
#include "mex.h"

#include "arm_rfft_fast_init_f32_simulink.c"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    uint16_t fftLen;
    arm_rfft_fast_instance_f32 S;
    int status;

    if (nrhs != 1)
        mexErrMsgIdAndTxt("arm_rfft_fast_init_f32_mex:nrhs", "Usage: S = arm_rfft_fast_init_f32_mex(fftLen)");
    if (nlhs > 1)
        mexErrMsgIdAndTxt("arm_rfft_fast_init_f32_mex:nlhs", "One output required.");

    fftLen = (uint16_t)mxGetScalar(prhs[0]);

    if (fftLen != 32 && fftLen != 64 && fftLen != 128 && fftLen != 256 &&
        fftLen != 512 && fftLen != 1024 && fftLen != 2048 && fftLen != 4096)
        mexErrMsgIdAndTxt("arm_rfft_fast_init_f32_mex:len",
            "Supported: 32,64,128,256,512,1024,2048,4096.");

    status = arm_rfft_fast_init_f32(&S, fftLen);

    plhs[0] = mxCreateStructMatrix(1, 1, 2, (const char *[]){"fftLenRFFT","cfftLen"});
    mxSetField(plhs[0], 0, "fftLenRFFT", mxCreateDoubleScalar((double)S.fftLenRFFT));
    mxSetField(plhs[0], 0, "cfftLen", mxCreateDoubleScalar((double)S.Sint.fftLen));

    arm_rfft_fast_init_f32_free(&S);
}