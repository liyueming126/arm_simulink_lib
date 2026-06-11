#include <stdint.h>
#include <string.h>
#include "mex.h"

typedef int16_t q15_t;
typedef int32_t q31_t;
typedef int64_t q63_t;

typedef struct {
    uint16_t numTaps;
    q31_t *pState;
    q31_t *pCoeffs;
    q31_t mu;
    uint8_t postShift;
    q31_t energy;
    q31_t x0;
    q31_t *recipTable;
} arm_lms_norm_instance_q31;

static void arm_lms_norm_init_q31(
    arm_lms_norm_instance_q31 *S, uint16_t numTaps, q31_t *pCoeffs, q31_t *pState,
    q31_t mu, uint32_t blockSize, uint8_t postShift)
{
    S->numTaps = numTaps;
    S->pCoeffs = pCoeffs;
    memset(pState, 0, (numTaps + (blockSize - 1u)) * sizeof(q31_t));
    S->postShift = postShift;
    S->pState = pState;
    S->mu = mu;
    S->recipTable = (q31_t *)0;
    S->energy = 0;
    S->x0 = 0;
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    uint16_t numTaps;
    int32_t muVal;
    uint32_t blockSize;
    uint8_t postShiftVal;
    (void)nlhs;

    if (nrhs < 1)
        mexErrMsgIdAndTxt("arm_lms_norm_init_q31_mex:invalidInput","Input: numTaps, [mu=0x40000000], [blockSize=1], [postShift=0]");

    if (mxIsDouble(prhs[0])) numTaps = (uint16_t)((double *)mxGetData(prhs[0]))[0];
    else if (mxIsInt32(prhs[0])) numTaps = (uint16_t)((int32_t *)mxGetData(prhs[0]))[0];
    else mexErrMsgIdAndTxt("arm_lms_norm_init_q31_mex:invalidType","numTaps must be double or int32");

    muVal = 0x40000000;
    if (nrhs >= 2 && !mxIsEmpty(prhs[1]))
    {
        if (mxIsDouble(prhs[1])) muVal = (int32_t)((double *)mxGetData(prhs[1]))[0];
        else if (mxIsInt32(prhs[1])) muVal = ((int32_t *)mxGetData(prhs[1]))[0];
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
        if (mxIsDouble(prhs[3])) postShiftVal = (uint8_t)((double *)mxGetData(prhs[3]))[0];
        else if (mxIsInt32(prhs[3])) postShiftVal = (uint8_t)((int32_t *)mxGetData(prhs[3]))[0];
    }

    plhs[0] = mxCreateNumericMatrix(1, (mwSize)numTaps, mxINT32_CLASS, mxREAL);
    memset(mxGetData(plhs[0]), 0, numTaps * sizeof(int32_t));
    plhs[1] = mxCreateNumericMatrix(1, (mwSize)(numTaps + blockSize - 1), mxINT32_CLASS, mxREAL);
    memset(mxGetData(plhs[1]), 0, (numTaps + blockSize - 1) * sizeof(int32_t));
    plhs[2] = mxCreateDoubleScalar((double)postShiftVal);
    plhs[3] = mxCreateDoubleScalar((double)muVal);
    plhs[4] = mxCreateDoubleScalar((double)numTaps);
    plhs[5] = mxCreateDoubleScalar((double)blockSize);
}