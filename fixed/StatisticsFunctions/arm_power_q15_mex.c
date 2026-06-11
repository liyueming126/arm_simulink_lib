#include <stdint.h>
#include "mex.h"

typedef int16_t q15_t;
typedef int64_t q63_t;

void arm_power_q15(
    q15_t *pSrc,
    uint32_t blockSize,
    q63_t *pResult)
{
    q63_t sum = 0;
    q15_t in;
    uint32_t blkCnt;

    blkCnt = blockSize;

    while (blkCnt > 0u)
    {
        in = *pSrc++;
        sum += ((int32_t)in * in);
        blkCnt--;
    }

    *pResult = sum;
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int16_t *pSrc;
    size_t blockSize;
    int64_t result;
    double dresult;

    if (nrhs != 1)
        mexErrMsgIdAndTxt("arm_power_q15:invalidInput", "1 input required.");
    if (nlhs > 1)
        mexErrMsgIdAndTxt("arm_power_q15:invalidOutput", "At most 1 output allowed.");
    if (!mxIsInt16(prhs[0]) || mxIsComplex(prhs[0]) || mxIsSparse(prhs[0]))
        mexErrMsgIdAndTxt("arm_power_q15:invalidInputType", "Input must be real int16.");

    blockSize = mxGetNumberOfElements(prhs[0]);
    pSrc = (int16_t *)mxGetData(prhs[0]);

    arm_power_q15(pSrc, (uint32_t)blockSize, &result);

    dresult = (double)result;
    plhs[0] = mxCreateDoubleScalar(dresult);
}