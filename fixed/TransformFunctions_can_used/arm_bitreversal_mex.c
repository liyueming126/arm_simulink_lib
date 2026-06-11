#include "mex.h"
#include <stdint.h>

#include "arm_bitreversal_simulink.c"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int16_t *pData, *pDataOut;
    uint32_t fftLen;
    uint16_t bitRevFactor;
    uint16_t *bitRevTable;
    size_t nElements;
    mwSize i;

    if (nrhs < 3)
        mexErrMsgTxt("Usage: y = arm_bitreversal_mex(data, fftLen, bitRevTable)");

    if (!mxIsInt16(prhs[0]) || mxIsComplex(prhs[0]) || mxIsSparse(prhs[0]))
        mexErrMsgTxt("data must be real int16.");

    nElements = mxGetNumberOfElements(prhs[0]);
    if (nElements != 2 * (size_t)mxGetScalar(prhs[1]))
        mexErrMsgTxt("data length must be 2*fftLen.");

    fftLen = (uint32_t)mxGetScalar(prhs[1]);
    bitRevFactor = (uint16_t)(fftLen >> 1);

    if (!mxIsUint16(prhs[2]) || mxIsComplex(prhs[2]) || mxIsSparse(prhs[2]))
        mexErrMsgTxt("bitRevTable must be real uint16.");

    bitRevTable = (uint16_t *)mxGetData(prhs[2]);

    plhs[0] = mxCreateNumericArray(mxGetNumberOfDimensions(prhs[0]),
                                    mxGetDimensions(prhs[0]),
                                    mxINT16_CLASS, mxREAL);
    pData = (int16_t *)mxGetData(prhs[0]);
    pDataOut = (int16_t *)mxGetData(plhs[0]);
    for (i = 0; i < (mwSize)nElements; i++)
        pDataOut[i] = pData[i];

    arm_bitreversal_q15((q15_t *)pDataOut, fftLen, bitRevFactor, bitRevTable);
}