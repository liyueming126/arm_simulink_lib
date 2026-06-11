#include <stdint.h>
#include <limits.h>
#include "mex.h"

typedef int8_t q7_t;

void arm_scale_q7(
    q7_t *pSrc,
    q7_t scaleFract,
    int8_t shift,
    q7_t *pDst,
    uint32_t blockSize)
{
    uint32_t blkCnt;
    int16_t val;
    int8_t kShift;

    blkCnt = blockSize;
    kShift = 7 - shift;

    while (blkCnt > 0u)
	{
		val = ((int16_t)*pSrc++ * (int16_t)scaleFract) >> kShift;

		if (val > 127)
			val = 127;
		else if (val < -128)
			val = -128;

		*pDst++ = (q7_t)val;
		blkCnt--;
	}
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int8_t *pSrc, *pDst;
    int8_t scaleFract;
    int8_t shift;
    size_t blockSize;

    if (nrhs != 3)
        mexErrMsgIdAndTxt("arm_scale_q7:invalidInput", "3 inputs required (vector, scaleFract, shift).");
    if (nlhs > 1)
        mexErrMsgIdAndTxt("arm_scale_q7:invalidOutput", "At most 1 output allowed.");
    if (!mxIsInt8(prhs[0]) || mxIsComplex(prhs[0]) || mxIsSparse(prhs[0]))
        mexErrMsgIdAndTxt("arm_scale_q7:invalidInputType", "Input 1 must be real int8.");
    if (!mxIsInt8(prhs[1]) || mxIsComplex(prhs[1]) || mxGetNumberOfElements(prhs[1]) != 1)
        mexErrMsgIdAndTxt("arm_scale_q7:invalidInputType", "Input 2 (scaleFract) must be a real int8 scalar.");
    if (!mxIsInt8(prhs[2]) || mxIsComplex(prhs[2]) || mxGetNumberOfElements(prhs[2]) != 1)
        mexErrMsgIdAndTxt("arm_scale_q7:invalidInputType", "Input 3 (shift) must be a real int8 scalar.");

    blockSize = mxGetNumberOfElements(prhs[0]);
    plhs[0] = mxCreateNumericArray(mxGetNumberOfDimensions(prhs[0]), mxGetDimensions(prhs[0]), mxINT8_CLASS, mxREAL);
    pSrc = (int8_t *)mxGetData(prhs[0]);
    scaleFract = *(int8_t *)mxGetData(prhs[1]);
    shift = *(int8_t *)mxGetData(prhs[2]);
    pDst = (int8_t *)mxGetData(plhs[0]);

    arm_scale_q7(pSrc, scaleFract, shift, pDst, (uint32_t)blockSize);
}