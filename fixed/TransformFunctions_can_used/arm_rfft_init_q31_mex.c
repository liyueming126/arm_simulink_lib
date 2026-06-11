#include "mex.h"
#include <stdint.h>

/* Include the actual implementation */
#include "arm_rfft_init_q31_simulink.c"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
    uint16_t fftLenReal;
    uint8_t ifftFlagR, bitReverseFlagR;
    arm_rfft_instance_q31 S;

    const char *fieldNames[] = {"fftLenReal","ifftFlagR","bitReverseFlagR","twidCoefRModifier"};

    if (nrhs < 3) mexErrMsgTxt("Usage: [S] = arm_rfft_init_q31_mex(fftLenReal, ifftFlagR, bitReverseFlagR)");

    fftLenReal = (uint16_t)mxGetScalar(prhs[0]);
    ifftFlagR = (uint8_t)mxGetScalar(prhs[1]);
    bitReverseFlagR = (uint8_t)mxGetScalar(prhs[2]);

    arm_rfft_init_q31(&S, fftLenReal, ifftFlagR, bitReverseFlagR);

    plhs[0] = mxCreateStructMatrix(1,1,4,fieldNames);
    mxSetField(plhs[0],0,"fftLenReal",mxCreateDoubleScalar(S.fftLenReal));
    mxSetField(plhs[0],0,"ifftFlagR",mxCreateDoubleScalar(S.ifftFlagR));
    mxSetField(plhs[0],0,"bitReverseFlagR",mxCreateDoubleScalar(S.bitReverseFlagR));
    mxSetField(plhs[0],0,"twidCoefRModifier",mxCreateDoubleScalar(S.twidCoefRModifier));
}