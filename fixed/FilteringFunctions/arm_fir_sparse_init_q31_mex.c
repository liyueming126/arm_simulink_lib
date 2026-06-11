#include <stdint.h>
#include <string.h>
#include "mex.h"

typedef int16_t q15_t;
typedef int32_t q31_t;
typedef int64_t q63_t;

typedef struct {
    uint16_t numTaps;
    uint16_t stateIndex;
    q31_t *pState;
    q31_t *pCoeffs;
    uint16_t maxDelay;
    int32_t *pTapDelay;
} arm_fir_sparse_instance_q31;

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    uint16_t numTaps, maxDelay;
    uint32_t blockSize;
    (void)nlhs;

    if (nrhs < 3)
        mexErrMsgIdAndTxt("arm_fir_sparse_init_q31_mex:invalidInput","Inputs: numTaps, maxDelay, blockSize");

    if (mxIsDouble(prhs[0])) numTaps = (uint16_t)((double *)mxGetData(prhs[0]))[0];
    else if (mxIsInt32(prhs[0])) numTaps = (uint16_t)((int32_t *)mxGetData(prhs[0]))[0];
    else mexErrMsgIdAndTxt("arm_fir_sparse_init_q31_mex:invalidType","numTaps must be double or int32");

    if (mxIsDouble(prhs[1])) maxDelay = (uint16_t)((double *)mxGetData(prhs[1]))[0];
    else if (mxIsInt32(prhs[1])) maxDelay = (uint16_t)((int32_t *)mxGetData(prhs[1]))[0];
    else mexErrMsgIdAndTxt("arm_fir_sparse_init_q31_mex:invalidType","maxDelay must be double or int32");

    if (mxIsDouble(prhs[2])) blockSize = (uint32_t)((double *)mxGetData(prhs[2]))[0];
    else if (mxIsInt32(prhs[2])) blockSize = (uint32_t)((int32_t *)mxGetData(prhs[2]))[0];
    else mexErrMsgIdAndTxt("arm_fir_sparse_init_q31_mex:invalidType","blockSize must be double or int32");

    plhs[0] = mxCreateNumericMatrix(1, (mwSize)numTaps, mxINT32_CLASS, mxREAL);
    memset(mxGetData(plhs[0]), 0, numTaps * sizeof(int32_t));
    plhs[1] = mxCreateNumericMatrix(1, (mwSize)(maxDelay + blockSize), mxINT32_CLASS, mxREAL);
    memset(mxGetData(plhs[1]), 0, (maxDelay + blockSize) * sizeof(int32_t));
    plhs[2] = mxCreateNumericMatrix(1, (mwSize)numTaps, mxINT32_CLASS, mxREAL);
    memset(mxGetData(plhs[2]), 0, numTaps * sizeof(int32_t));
    plhs[3] = mxCreateDoubleScalar((double)numTaps);
    plhs[4] = mxCreateDoubleScalar((double)maxDelay);
    plhs[5] = mxCreateDoubleScalar((double)blockSize);
}