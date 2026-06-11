#include <stdint.h>
#include <string.h>
#include <limits.h>
#include "mex.h"

typedef int16_t q15_t;
typedef int32_t q31_t;

typedef struct {
    uint16_t numTaps;
    q15_t *pState;
    q15_t *pCoeffs;
} arm_fir_instance_q15;

static void arm_fir_fast_q15(
    const arm_fir_instance_q15 *S,
    q15_t *pSrc,
    q15_t *pDst,
    uint32_t blockSize)
{
    q15_t *pState = S->pState;
    q15_t *pCoeffs = S->pCoeffs;
    q15_t *pStateCurnt;
    q15_t *px;
    q15_t *pb;
    q31_t acc;
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
        tapCnt = numTaps >> 1u;

        do
        {
            acc += (q31_t)*px++ * *pb++;
            acc += (q31_t)*px++ * *pb++;
            tapCnt--;
        } while (tapCnt > 0u);

        if (acc > 0)
        {
            acc = (acc >> 15);
            *pDst++ = (q15_t)(acc > 32767 ? 32767 : acc);
        }
        else
        {
            acc = (acc >> 15);
            *pDst++ = (q15_t)(acc < -32768 ? -32768 : acc);
        }

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
    int16_t *pSrc, *pDst, *pCoeffs, *pState;
    size_t blockSize, numTaps;
    arm_fir_instance_q15 S;
    mxArray *stateArray;
    size_t stateLen;

    if (nrhs != 3)
    {
        mexErrMsgIdAndTxt("arm_fir_fast_q15_mex:invalidInput",
            "Three inputs required: x (int16 array), coeffs (int16 array), state (int16 array or empty).");
    }

    if (nlhs > 2)
    {
        mexErrMsgIdAndTxt("arm_fir_fast_q15_mex:invalidOutput",
            "At most two outputs: y and updated state.");
    }

    if (!mxIsInt16(prhs[0]) || mxIsComplex(prhs[0]))
    {
        mexErrMsgIdAndTxt("arm_fir_fast_q15_mex:invalidInputType",
            "First input must be a real int16 array.");
    }

    if (!mxIsInt16(prhs[1]) || mxIsComplex(prhs[1]))
    {
        mexErrMsgIdAndTxt("arm_fir_fast_q15_mex:invalidInputType",
            "Second input must be a real int16 array (coefficients).");
    }

    if (!mxIsInt16(prhs[2]) || mxIsComplex(prhs[2]))
    {
        mexErrMsgIdAndTxt("arm_fir_fast_q15_mex:invalidInputType",
            "Third input must be a real int16 state array, or zero-length.");
    }

    pSrc = (int16_t *)mxGetData(prhs[0]);
    blockSize = mxGetNumberOfElements(prhs[0]);

    pCoeffs = (int16_t *)mxGetData(prhs[1]);
    numTaps = mxGetNumberOfElements(prhs[1]);

    stateLen = mxGetNumberOfElements(prhs[2]);

    if (stateLen == 0)
    {
        stateArray = mxCreateNumericMatrix(1, (mwSize)(numTaps + blockSize - 1), mxINT16_CLASS, mxREAL);
        pState = (int16_t *)mxGetData(stateArray);
        memset(pState, 0, (numTaps + blockSize - 1) * sizeof(int16_t));
    }
    else
    {
        stateArray = mxDuplicateArray(prhs[2]);
        pState = (int16_t *)mxGetData(stateArray);
    }

    S.numTaps = (uint16_t)numTaps;
    S.pCoeffs = pCoeffs;
    S.pState = pState;

    plhs[0] = mxCreateNumericMatrix(1, (mwSize)blockSize, mxINT16_CLASS, mxREAL);
    pDst = (int16_t *)mxGetData(plhs[0]);

    arm_fir_fast_q15(&S, pSrc, pDst, (uint32_t)blockSize);

    if (nlhs >= 2)
    {
        plhs[1] = stateArray;
    }
    else
    {
        mxDestroyArray(stateArray);
    }
}