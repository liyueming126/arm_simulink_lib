#include <stdint.h>
#include "mex.h"

typedef int32_t q31_t;
typedef int64_t q63_t;

void arm_power_q31(
    q31_t *pSrc,
    uint32_t blockSize,
    q63_t *pResult)
{
    q63_t sum = 0;
    q31_t in;
    uint32_t blkCnt;

    blkCnt = blockSize;

    while (blkCnt > 0u)
    {
        in = *pSrc++;
        sum += ((q63_t)in * in) >> 14u;
        blkCnt--;
    }

    *pResult = sum;
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int32_t *pSrc;
    size_t blockSize;
    int64_t result;
    double dresult;

    if (nrhs != 1)
        mexErrMsgIdAndTxt("arm_power_q31:invalidInput", "1 input required.");
    if (nlhs > 1)
        mexErrMsgIdAndTxt("arm_power_q31:invalidOutput", "At most 1 output allowed.");
    if (!mxIsInt32(prhs[0]) || mxIsComplex(prhs[0]) || mxIsSparse(prhs[0]))
        mexErrMsgIdAndTxt("arm_power_q31:invalidInputType", "Input must be real int32.");

    blockSize = mxGetNumberOfElements(prhs[0]);
    pSrc = (int32_t *)mxGetData(prhs[0]);

    arm_power_q31(pSrc, (uint32_t)blockSize, &result);

    dresult = (double)result;
    plhs[0] = mxCreateDoubleScalar(dresult);
}