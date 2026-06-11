#include <stdint.h>
#include <string.h>
#include "mex.h"

typedef int8_t q7_t;

typedef struct {
    uint16_t numTaps;
    q7_t *pState;
    q7_t *pCoeffs;
} arm_fir_instance_q7;

static void arm_fir_q7(
    const arm_fir_instance_q7 *S,
    q7_t *pSrc,
    q7_t *pDst,
    uint32_t blockSize)
{
    q7_t *pState = S->pState;
    q7_t *pCoeffs = S->pCoeffs;
    q7_t *pStateCurnt;
    q7_t *px;
    q7_t *pb;
    int32_t acc;
    uint32_t numTaps = S->numTaps;
    uint32_t tapCnt, blkCnt;

    pStateCurnt = &(S->pState[(numTaps - 1u)]);

    blkCnt = blockSize;

    while (blkCnt > 0u)
    {
        *pStateCurnt++ = *pSrc++;
        acc = 0;
        px = pState;
        pb = pCoeffs;
        tapCnt = numTaps;

        do
        {
            acc += (int16_t)*px++ * (int16_t)*pb++;
            tapCnt--;
        } while (tapCnt > 0u);

        acc = acc >> 7;
        if (acc > 127) acc = 127;
        if (acc < -128) acc = -128;
        *pDst++ = (q7_t)acc;

        pState = pState + 1;
        blkCnt--;
    }

    pStateCurnt = S->pState;
    tapCnt = (numTaps - 1u);

    while (tapCnt > 0u)
    {
        *pStateCurnt++ = *pState++;
        tapCnt--;
    }
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int8_t *pSrc, *pDst, *pCoeffs, *pState;
    size_t blockSize, numTaps, stateLen;
    arm_fir_instance_q7 S;
    mxArray *stateArray;

    if (nrhs != 3)
    {
        mexErrMsgIdAndTxt("arm_fir_q7_mex:invalidInput",
            "Three inputs required: x (int8 array), coeffs (int8 array), state (int8 array or empty).");
    }

    if (nlhs > 2)
    {
        mexErrMsgIdAndTxt("arm_fir_q7_mex:invalidOutput",
            "At most two outputs: y (filter output) and updated state.");
    }

    if (!mxIsInt8(prhs[0]) || mxIsComplex(prhs[0]))
    {
        mexErrMsgIdAndTxt("arm_fir_q7_mex:invalidInputType",
            "First input must be a real int8 array (input signal).");
    }

    if (!mxIsInt8(prhs[1]) || mxIsComplex(prhs[1]))
    {
        mexErrMsgIdAndTxt("arm_fir_q7_mex:invalidInputType",
            "Second input must be a real int8 array (filter coefficients).");
    }

    if (!mxIsInt8(prhs[2]) || mxIsComplex(prhs[2]))
    {
        mexErrMsgIdAndTxt("arm_fir_q7_mex:invalidInputType",
            "Third input must be a real int8 state array, or zero-length.");
    }

    pSrc = (int8_t *)mxGetData(prhs[0]);
    blockSize = mxGetNumberOfElements(prhs[0]);

    pCoeffs = (int8_t *)mxGetData(prhs[1]);
    numTaps = mxGetNumberOfElements(prhs[1]);

    pState = (int8_t *)mxGetData(prhs[2]);
    stateLen = mxGetNumberOfElements(prhs[2]);

    if (stateLen == 0)
    {
        stateArray = mxCreateNumericMatrix(1, (mwSize)(numTaps + blockSize - 1), mxINT8_CLASS, mxREAL);
        pState = (int8_t *)mxGetData(stateArray);
        memset(pState, 0, (numTaps + blockSize - 1) * sizeof(int8_t));
    }
    else
    {
        stateArray = mxDuplicateArray(prhs[2]);
        pState = (int8_t *)mxGetData(stateArray);
    }

    S.numTaps = (uint16_t)numTaps;
    S.pCoeffs = pCoeffs;
    S.pState = pState;

    plhs[0] = mxCreateNumericMatrix(1, (mwSize)blockSize, mxINT8_CLASS, mxREAL);
    pDst = (int8_t *)mxGetData(plhs[0]);

    arm_fir_q7(&S, pSrc, pDst, (uint32_t)blockSize);

    if (nlhs >= 2)
    {
        plhs[1] = stateArray;
    }
    else
    {
        mxDestroyArray(stateArray);
    }
}