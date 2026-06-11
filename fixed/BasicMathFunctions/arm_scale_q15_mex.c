#include <stdint.h>
#include <limits.h>
#include "mex.h"

typedef int16_t q15_t;

void arm_scale_q15(
    q15_t *pSrc,
    q15_t scaleFract,
    int8_t shift,
    q15_t *pDst,
    uint32_t blockSize)
{
    uint32_t blkCnt;
    int32_t val;
    int8_t kShift;

    blkCnt = blockSize;
    kShift = 15 - shift;

    while (blkCnt > 0u)
	{
		val = ((int32_t)*pSrc++ * (int32_t)scaleFract) >> kShift;

		if (val > 32767)
			val = 32767;
		else if (val < -32768)
			val = -32768;

		*pDst++ = (q15_t)val;
		blkCnt--;
	}
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int16_t *pSrc, *pDst;
    int16_t scaleFract;
    int8_t shift;
    size_t blockSize;

    if (nrhs != 3)
        mexErrMsgIdAndTxt("arm_scale_q15:invalidInput", "3 inputs required (vector, scaleFract, shift).");
    if (nlhs > 1)
        mexErrMsgIdAndTxt("arm_scale_q15:invalidOutput", "At most 1 output allowed.");
    if (!mxIsInt16(prhs[0]) || mxIsComplex(prhs[0]) || mxIsSparse(prhs[0]))
        mexErrMsgIdAndTxt("arm_scale_q15:invalidInputType", "Input 1 must be real int16.");
    if (!mxIsInt16(prhs[1]) || mxIsComplex(prhs[1]) || mxGetNumberOfElements(prhs[1]) != 1)
        mexErrMsgIdAndTxt("arm_scale_q15:invalidInputType", "Input 2 (scaleFract) must be a real int16 scalar.");
    if (!mxIsInt8(prhs[2]) || mxIsComplex(prhs[2]) || mxGetNumberOfElements(prhs[2]) != 1)
        mexErrMsgIdAndTxt("arm_scale_q15:invalidInputType", "Input 3 (shift) must be a real int8 scalar.");

    blockSize = mxGetNumberOfElements(prhs[0]);
    plhs[0] = mxCreateNumericArray(mxGetNumberOfDimensions(prhs[0]), mxGetDimensions(prhs[0]), mxINT16_CLASS, mxREAL);
    pSrc = (int16_t *)mxGetData(prhs[0]);
    scaleFract = *(int16_t *)mxGetData(prhs[1]);
    shift = *(int8_t *)mxGetData(prhs[2]);
    pDst = (int16_t *)mxGetData(plhs[0]);

    arm_scale_q15(pSrc, scaleFract, shift, pDst, (uint32_t)blockSize);
}