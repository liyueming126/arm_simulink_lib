#include "mex.h"
#include <stdint.h>

/* Include the actual implementation */
#include "arm_dct4_init_q31_simulink.c"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
    uint16_t N;
    q31_t normalize;
    arm_dct4_instance_q31 S;

    const char *fieldNames[] = {"N","Nby2","normalize"};

    if (nrhs < 2) mexErrMsgTxt("Usage: [S] = arm_dct4_init_q31_mex(N, normalize)");

    N = (uint16_t)mxGetScalar(prhs[0]);
    normalize = (q31_t)mxGetScalar(prhs[1]);

    arm_dct4_init_q31(&S, N, normalize);

    plhs[0] = mxCreateStructMatrix(1,1,3,fieldNames);
    mxSetField(plhs[0],0,"N",mxCreateDoubleScalar(S.N));
    mxSetField(plhs[0],0,"Nby2",mxCreateDoubleScalar(S.Nby2));
    mxSetField(plhs[0],0,"normalize",mxCreateDoubleScalar(S.normalize));
}