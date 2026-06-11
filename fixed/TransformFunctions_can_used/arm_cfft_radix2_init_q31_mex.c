#include "mex.h"
#include <stdint.h>

/* Include the actual implementation */
#include "arm_cfft_radix2_init_q31_simulink.c"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
    uint16_t fftLen;
    uint8_t ifftFlag, bitReverseFlag;
    arm_cfft_radix2_instance_q31 S;

    const char *field_names[] = {"fftLen","ifftFlag","bitReverseFlag","twidCoefModifier","bitRevFactor"};

    if (nrhs < 3) mexErrMsgTxt("Usage: [S] = arm_cfft_radix2_init_q31_mex(fftLen, ifftFlag, bitReverseFlag)");

    fftLen         = (uint16_t)mxGetScalar(prhs[0]);
    ifftFlag       = (uint8_t)mxGetScalar(prhs[1]);
    bitReverseFlag = (uint8_t)mxGetScalar(prhs[2]);

    arm_cfft_radix2_init_q31(&S, fftLen, ifftFlag, bitReverseFlag);

    plhs[0] = mxCreateStructMatrix(1,1,5,field_names);
    mxSetField(plhs[0],0,"fftLen",mxCreateDoubleScalar(S.fftLen));
    mxSetField(plhs[0],0,"ifftFlag",mxCreateDoubleScalar(S.ifftFlag));
    mxSetField(plhs[0],0,"bitReverseFlag",mxCreateDoubleScalar(S.bitReverseFlag));
    mxSetField(plhs[0],0,"twidCoefModifier",mxCreateDoubleScalar(S.twidCoefModifier));
    mxSetField(plhs[0],0,"bitRevFactor",mxCreateDoubleScalar(S.bitRevFactor));
}