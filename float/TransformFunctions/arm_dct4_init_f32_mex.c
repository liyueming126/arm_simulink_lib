/* ----------------------------------------------------------------------
 * arm_dct4_init_f32_mex.c
 * MEX wrapper for arm_dct4_init_f32.
 * Usage: S = arm_dct4_init_f32_mex(N, Nby2, normalize)
 * -------------------------------------------------------------------- */
#include <stdint.h>
#include <stdlib.h>
#include "mex.h"

#include "arm_dct4_init_f32_simulink.c"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    uint16_t N, Nby2;
    float normalize;
    arm_dct4_instance_f32 S;
    int status;

    if (nrhs != 3)
        mexErrMsgIdAndTxt("arm_dct4_init_f32_mex:nrhs", "Usage: S = arm_dct4_init_f32_mex(N, Nby2, normalize)");
    if (nlhs > 1)
        mexErrMsgIdAndTxt("arm_dct4_init_f32_mex:nlhs", "One output required.");

    N = (uint16_t)mxGetScalar(prhs[0]);
    Nby2 = (uint16_t)mxGetScalar(prhs[1]);
    normalize = (float)mxGetScalar(prhs[2]);

    status = arm_dct4_init_f32(&S, N, Nby2, normalize);
    if (status != 0)
        mexErrMsgIdAndTxt("arm_dct4_init_f32_mex:alloc", "Failed to allocate tables.");

    plhs[0] = mxCreateStructMatrix(1, 1, 3, (const char *[]){"N","Nby2","normalize"});
    mxSetField(plhs[0], 0, "N", mxCreateDoubleScalar((double)S.N));
    mxSetField(plhs[0], 0, "Nby2", mxCreateDoubleScalar((double)S.Nby2));
    mxSetField(plhs[0], 0, "normalize", mxCreateDoubleScalar((double)S.normalize));

    arm_dct4_init_f32_free(&S);
}