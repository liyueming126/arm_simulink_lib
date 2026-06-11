#include <stdint.h>
#include <string.h>
#include "mex.h"

typedef int16_t q15_t;
typedef int32_t q31_t;
typedef int64_t q63_t;

typedef struct {
    uint16_t numTaps;
    q15_t *pState;
    q15_t *pCoeffs;
    q15_t mu;
    uint32_t postShift;
} arm_lms_instance_q15;

static void arm_lms_init_q15(
    arm_lms_instance_q15 *S, uint16_t numTaps, q15_t *pCoeffs, q15_t *pState,
    q15_t mu, uint32_t blockSize, uint32_t postShift)
{
    S->numTaps = numTaps;
    S->pCoeffs = pCoeffs;
    memset(pState, 0, (numTaps + (blockSize - 1u)) * sizeof(q15_t));
    S->pState = pState;
    S->mu = mu;
    S->postShift = postShift;
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    uint16_t numTaps;
    int16_t muVal;
    uint32_t blockSize, postShiftVal;
    (void)nlhs;

    if (nrhs < 1)
        mexErrMsgIdAndTxt("arm_lms_init_q15_mex:invalidInput",
            "Inputs: numTaps(uint16), [mu(int16=512)], [blockSize(uint32)=1], [postShift(uint32)=0]");

    if (mxIsDouble(prhs[0])) numTaps = (uint16_t)((double *)mxGetData(prhs[0]))[0];
    else if (mxIsInt32(prhs[0])) numTaps = (uint16_t)((int32_t *)mxGetData(prhs[0]))[0];
    else mexErrMsgIdAndTxt("arm_lms_init_q15_mex:invalidType","numTaps must be double or int32");

    muVal = 512;
    if (nrhs >= 2 && !mxIsEmpty(prhs[1]))
    {
        if (mxIsDouble(prhs[1])) muVal = (int16_t)((double *)mxGetData(prhs[1]))[0];
        else if (mxIsInt16(prhs[1])) muVal = ((int16_t *)mxGetData(prhs[1]))[0];
    }

    blockSize = 1;
    if (nrhs >= 3 && !mxIsEmpty(prhs[2]))
    {
        if (mxIsDouble(prhs[2])) blockSize = (uint32_t)((double *)mxGetData(prhs[2]))[0];
        else if (mxIsInt32(prhs[2])) blockSize = (uint32_t)((int32_t *)mxGetData(prhs[2]))[0];
    }

    postShiftVal = 0;
    if (nrhs >= 4 && !mxIsEmpty(prhs[3]))
    {
        if (mxIsDouble(prhs[3])) postShiftVal = (uint32_t)((double *)mxGetData(prhs[3]))[0];
        else if (mxIsInt32(prhs[3])) postShiftVal = (uint32_t)((int32_t *)mxGetData(prhs[3]))[0];
    }

    plhs[0] = mxCreateNumericMatrix(1, (mwSize)numTaps, mxINT16_CLASS, mxREAL);
    memset(mxGetData(plhs[0]), 0, numTaps * sizeof(int16_t));
    plhs[1] = mxCreateNumericMatrix(1, (mwSize)(numTaps + blockSize - 1), mxINT16_CLASS, mxREAL);
    memset(mxGetData(plhs[1]), 0, (numTaps + blockSize - 1) * sizeof(int16_t));
    plhs[2] = mxCreateDoubleScalar((double)postShiftVal);
    plhs[3] = mxCreateDoubleScalar((double)muVal);
    plhs[4] = mxCreateDoubleScalar((double)numTaps);
    plhs[5] = mxCreateDoubleScalar((double)blockSize);
}