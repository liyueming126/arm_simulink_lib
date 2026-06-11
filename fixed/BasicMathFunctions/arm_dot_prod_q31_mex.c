#include <stdint.h>
#include <limits.h>
#include "mex.h"

typedef int32_t q31_t;
typedef int64_t q63_t;

void arm_dot_prod_q31(
    q31_t *pSrcA,
    q31_t *pSrcB,
    uint32_t blockSize,
    q63_t *result)
{
    q63_t sum = 0;
    uint32_t blkCnt;

    blkCnt = blockSize;

    while (blkCnt > 0u)
    {
        sum += ((q63_t)*pSrcA++ * (q63_t)*pSrcB++) >> 14u;
        blkCnt--;
    }

    *result = sum;
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int32_t *pSrcA, *pSrcB;
    size_t blockSize;
    int64_t result;
    double dresult;

    if (nrhs != 2)
        mexErrMsgIdAndTxt("arm_dot_prod_q31:invalidInput", "2 inputs required.");
    if (nlhs > 1)
        mexErrMsgIdAndTxt("arm_dot_prod_q31:invalidOutput", "At most 1 output allowed.");
    if (!mxIsInt32(prhs[0]) || mxIsComplex(prhs[0]) || mxIsSparse(prhs[0]))
        mexErrMsgIdAndTxt("arm_dot_prod_q31:invalidInputType", "Input 1 must be real int32.");
    if (!mxIsInt32(prhs[1]) || mxIsComplex(prhs[1]) || mxIsSparse(prhs[1]))
        mexErrMsgIdAndTxt("arm_dot_prod_q31:invalidInputType", "Input 2 must be real int32.");

    blockSize = mxGetNumberOfElements(prhs[0]);
    if (mxGetNumberOfElements(prhs[1]) != blockSize)
        mexErrMsgIdAndTxt("arm_dot_prod_q31:invalidInput", "Inputs must have the same size.");

    pSrcA = (int32_t *)mxGetData(prhs[0]);
    pSrcB = (int32_t *)mxGetData(prhs[1]);

    arm_dot_prod_q31(pSrcA, pSrcB, (uint32_t)blockSize, &result);

    dresult = (double)result;
    plhs[0] = mxCreateDoubleScalar(dresult);
}