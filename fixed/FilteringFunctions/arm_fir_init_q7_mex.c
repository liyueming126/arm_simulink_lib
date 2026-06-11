#include <stdint.h>
#include <string.h>
#include "mex.h"

typedef int8_t q7_t;

typedef struct {
    uint16_t numTaps;
    q7_t *pState;
    q7_t *pCoeffs;
} arm_fir_instance_q7;

static void arm_fir_init_q7(
    arm_fir_instance_q7 *S,
    uint16_t numTaps,
    q7_t *pCoeffs,
    q7_t *pState,
    uint32_t blockSize)
{
    S->numTaps = numTaps;
    S->pCoeffs = pCoeffs;
    memset(pState, 0, (numTaps + (blockSize - 1u)) * sizeof(q7_t));
    S->pState = pState;
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int8_t *pCoeffs, *pState;
    size_t numTaps, blockSize;

    if (nrhs < 2 || nrhs > 3)
    {
        mexErrMsgIdAndTxt("arm_fir_init_q7_mex:invalidInput",
            "Inputs: numTaps, coeffs (int8 array), [blockSize].");
    }

    numTaps = (size_t)mxGetScalar(prhs[0]);

    if (!mxIsInt8(prhs[1]) || mxIsComplex(prhs[1]))
    {
        mexErrMsgIdAndTxt("arm_fir_init_q7_mex:invalidInputType",
            "Second input must be a real int8 array (coefficients).");
    }

    pCoeffs = (int8_t *)mxGetData(prhs[1]);

    if (nrhs >= 3)
    {
        blockSize = (size_t)mxGetScalar(prhs[2]);
    }
    else
    {
        blockSize = 1;
    }

    plhs[0] = mxCreateNumericMatrix(1, (mwSize)(numTaps + blockSize - 1), mxINT8_CLASS, mxREAL);
    pState = (int8_t *)mxGetData(plhs[0]);

    arm_fir_instance_q7 S;
    arm_fir_init_q7(&S, (uint16_t)numTaps, pCoeffs, pState, (uint32_t)blockSize);
}