#include <stdint.h>
#include <string.h>
#include "mex.h"

typedef int32_t q31_t;

typedef struct {
    uint16_t numTaps;
    q31_t *pState;
    q31_t *pCoeffs;
} arm_fir_instance_q31;

static void arm_fir_init_q31(
    arm_fir_instance_q31 *S,
    uint16_t numTaps,
    q31_t *pCoeffs,
    q31_t *pState,
    uint32_t blockSize)
{
    S->numTaps = numTaps;
    S->pCoeffs = pCoeffs;
    memset(pState, 0, (blockSize + ((uint32_t)numTaps - 1u)) * sizeof(q31_t));
    S->pState = pState;
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int32_t *pCoeffs, *pState;
    size_t numTaps, blockSize;
    arm_fir_instance_q31 S;

    if (nrhs < 2 || nrhs > 3)
    {
        mexErrMsgIdAndTxt("arm_fir_init_q31_mex:invalidInput",
            "Inputs: numTaps, coeffs (int32 array), [blockSize].");
    }

    numTaps = (size_t)mxGetScalar(prhs[0]);

    if (!mxIsInt32(prhs[1]) || mxIsComplex(prhs[1]))
    {
        mexErrMsgIdAndTxt("arm_fir_init_q31_mex:invalidInputType",
            "Second input must be a real int32 array (coefficients).");
    }

    pCoeffs = (int32_t *)mxGetData(prhs[1]);

    if (nrhs >= 3)
    {
        blockSize = (size_t)mxGetScalar(prhs[2]);
    }
    else
    {
        blockSize = 1;
    }

    plhs[0] = mxCreateNumericMatrix(1, (mwSize)(blockSize + numTaps - 1), mxINT32_CLASS, mxREAL);
    pState = (int32_t *)mxGetData(plhs[0]);

    arm_fir_init_q31(&S, (uint16_t)numTaps, pCoeffs, pState, (uint32_t)blockSize);
}