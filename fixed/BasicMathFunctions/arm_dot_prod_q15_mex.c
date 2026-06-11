#include <stdint.h>
#include <limits.h>
#include "mex.h"

typedef int16_t q15_t;
typedef int64_t q63_t;

void arm_dot_prod_q15(
    q15_t *pSrcA,
    q15_t *pSrcB,
    uint32_t blockSize,
    q63_t *result)
{
    q63_t sum = 0;
    uint32_t blkCnt;

    blkCnt = blockSize;

    while (blkCnt > 0u)
    {
        sum += (q63_t)((int32_t)*pSrcA++ * (int32_t)*pSrcB++);
        blkCnt--;
    }

    *result = sum;
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int16_t *pSrcA, *pSrcB;
    size_t blockSize;
    int64_t result;
    double dresult;

    if (nrhs != 2)
        mexErrMsgIdAndTxt("arm_dot_prod_q15:invalidInput", "2 inputs required.");
    if (nlhs > 1)
        mexErrMsgIdAndTxt("arm_dot_prod_q15:invalidOutput", "At most 1 output allowed.");
    if (!mxIsInt16(prhs[0]) || mxIsComplex(prhs[0]) || mxIsSparse(prhs[0]))
        mexErrMsgIdAndTxt("arm_dot_prod_q15:invalidInputType", "Input 1 must be real int16.");
    if (!mxIsInt16(prhs[1]) || mxIsComplex(prhs[1]) || mxIsSparse(prhs[1]))
        mexErrMsgIdAndTxt("arm_dot_prod_q15:invalidInputType", "Input 2 must be real int16.");

    blockSize = mxGetNumberOfElements(prhs[0]);
    if (mxGetNumberOfElements(prhs[1]) != blockSize)
        mexErrMsgIdAndTxt("arm_dot_prod_q15:invalidInput", "Inputs must have the same size.");

    pSrcA = (int16_t *)mxGetData(prhs[0]);
    pSrcB = (int16_t *)mxGetData(prhs[1]);

    arm_dot_prod_q15(pSrcA, pSrcB, (uint32_t)blockSize, &result);

    dresult = (double)result;
    plhs[0] = mxCreateDoubleScalar(dresult);
}