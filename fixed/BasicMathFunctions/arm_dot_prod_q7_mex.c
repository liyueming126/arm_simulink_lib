#include <stdint.h>
#include <limits.h>
#include "mex.h"

typedef int8_t q7_t;
typedef int32_t q31_t;

void arm_dot_prod_q7(
    q7_t *pSrcA,
    q7_t *pSrcB,
    uint32_t blockSize,
    q31_t *result)
{
    q31_t sum = 0;
    uint32_t blkCnt;

    blkCnt = blockSize;

    while (blkCnt > 0u)
    {
        sum += (q31_t)((int16_t)*pSrcA++ * (int16_t)*pSrcB++);
        blkCnt--;
    }

    *result = sum;
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int8_t *pSrcA, *pSrcB;
    size_t blockSize;
    int32_t result;
    double dresult;

    if (nrhs != 2)
        mexErrMsgIdAndTxt("arm_dot_prod_q7:invalidInput", "2 inputs required.");
    if (nlhs > 1)
        mexErrMsgIdAndTxt("arm_dot_prod_q7:invalidOutput", "At most 1 output allowed.");
    if (!mxIsInt8(prhs[0]) || mxIsComplex(prhs[0]) || mxIsSparse(prhs[0]))
        mexErrMsgIdAndTxt("arm_dot_prod_q7:invalidInputType", "Input 1 must be real int8.");
    if (!mxIsInt8(prhs[1]) || mxIsComplex(prhs[1]) || mxIsSparse(prhs[1]))
        mexErrMsgIdAndTxt("arm_dot_prod_q7:invalidInputType", "Input 2 must be real int8.");

    blockSize = mxGetNumberOfElements(prhs[0]);
    if (mxGetNumberOfElements(prhs[1]) != blockSize)
        mexErrMsgIdAndTxt("arm_dot_prod_q7:invalidInput", "Inputs must have the same size.");

    pSrcA = (int8_t *)mxGetData(prhs[0]);
    pSrcB = (int8_t *)mxGetData(prhs[1]);

    arm_dot_prod_q7(pSrcA, pSrcB, (uint32_t)blockSize, &result);

    dresult = (double)result;
    plhs[0] = mxCreateDoubleScalar(dresult);
}