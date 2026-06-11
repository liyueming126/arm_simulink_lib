#include <stdint.h>
#include <string.h>
#include "mex.h"

typedef int16_t q15_t;
typedef int32_t arm_status;
#define ARM_MATH_SUCCESS 0
#define ARM_MATH_ARGUMENT_ERROR 1

typedef struct {
    uint16_t numTaps;
    q15_t *pState;
    q15_t *pCoeffs;
} arm_fir_instance_q15;

static arm_status arm_fir_init_q15(
    arm_fir_instance_q15 *S,
    uint16_t numTaps,
    q15_t *pCoeffs,
    q15_t *pState,
    uint32_t blockSize)
{
    if (numTaps & 0x1u)
    {
        return ARM_MATH_ARGUMENT_ERROR;
    }

    S->numTaps = numTaps;
    S->pCoeffs = pCoeffs;
    memset(pState, 0, (numTaps + blockSize) * sizeof(q15_t));
    S->pState = pState;

    return ARM_MATH_SUCCESS;
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int16_t *pCoeffs, *pState;
    size_t numTaps, blockSize;
    arm_fir_instance_q15 S;
    arm_status status;

    if (nrhs < 2 || nrhs > 3)
    {
        mexErrMsgIdAndTxt("arm_fir_init_q15_mex:invalidInput",
            "Inputs: numTaps, coeffs (int16 array), [blockSize].");
    }

    numTaps = (size_t)mxGetScalar(prhs[0]);

    if (!mxIsInt16(prhs[1]) || mxIsComplex(prhs[1]))
    {
        mexErrMsgIdAndTxt("arm_fir_init_q15_mex:invalidInputType",
            "Second input must be a real int16 array (coefficients).");
    }

    pCoeffs = (int16_t *)mxGetData(prhs[1]);

    if (nrhs >= 3)
    {
        blockSize = (size_t)mxGetScalar(prhs[2]);
    }
    else
    {
        blockSize = 1;
    }

    S.numTaps = (uint16_t)numTaps;
    S.pCoeffs = pCoeffs;

    plhs[0] = mxCreateNumericMatrix(1, (mwSize)(numTaps + blockSize), mxINT16_CLASS, mxREAL);
    pState = (int16_t *)mxGetData(plhs[0]);

    status = arm_fir_init_q15(&S, (uint16_t)numTaps, pCoeffs, pState, (uint32_t)blockSize);

    if (status == ARM_MATH_ARGUMENT_ERROR)
    {
        mxDestroyArray(plhs[0]);
        mexErrMsgIdAndTxt("arm_fir_init_q15_mex:argError",
            "numTaps must be even and >= 4.");
    }
}