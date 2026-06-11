#include <stdint.h>
#include <string.h>
#include <limits.h>
#include "mex.h"

typedef int32_t q31_t;
typedef int64_t q63_t;

typedef struct {
    uint16_t numTaps;
    q31_t *pState;
    q31_t *pCoeffs;
} arm_fir_instance_q31;

static void arm_fir_q31(
    const arm_fir_instance_q31 *S,
    q31_t *pSrc,
    q31_t *pDst,
    uint32_t blockSize)
{
    q31_t *pState = S->pState;
    q31_t *pCoeffs = S->pCoeffs;
    q31_t *pStateCurnt;
    q31_t *px;
    q31_t *pb;
    q63_t acc;
    uint32_t numTaps = S->numTaps;
    uint32_t i, tapCnt, blkCnt;

    pStateCurnt = &(S->pState[(numTaps - 1u)]);

    blkCnt = blockSize;

    while (blkCnt > 0u)
    {
        *pStateCurnt++ = *pSrc++;
        acc = 0;
        px = pState;
        pb = pCoeffs;
        i = numTaps;

        do
        {
            acc += (q63_t)*px++ * *pb++;
            i--;
        } while (i > 0u);

        *pDst++ = (q31_t)(acc >> 31u);

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
    int32_t *pSrc, *pDst, *pCoeffs, *pState;
    size_t blockSize, numTaps;
    arm_fir_instance_q31 S;
    mxArray *stateArray;
    size_t stateLen;

    if (nrhs != 3)
    {
        mexErrMsgIdAndTxt("arm_fir_q31_mex:invalidInput",
            "Three inputs required: x (int32 array), coeffs (int32 array), state (int32 array or empty).");
    }

    if (nlhs > 2)
    {
        mexErrMsgIdAndTxt("arm_fir_q31_mex:invalidOutput",
            "At most two outputs: y and updated state.");
    }

    if (!mxIsInt32(prhs[0]) || mxIsComplex(prhs[0]))
    {
        mexErrMsgIdAndTxt("arm_fir_q31_mex:invalidInputType",
            "First input must be a real int32 array.");
    }

    if (!mxIsInt32(prhs[1]) || mxIsComplex(prhs[1]))
    {
        mexErrMsgIdAndTxt("arm_fir_q31_mex:invalidInputType",
            "Second input must be a real int32 array (coefficients).");
    }

    if (!mxIsInt32(prhs[2]) || mxIsComplex(prhs[2]))
    {
        mexErrMsgIdAndTxt("arm_fir_q31_mex:invalidInputType",
            "Third input must be a real int32 state array, or zero-length.");
    }

    pSrc = (int32_t *)mxGetData(prhs[0]);
    blockSize = mxGetNumberOfElements(prhs[0]);

    pCoeffs = (int32_t *)mxGetData(prhs[1]);
    numTaps = mxGetNumberOfElements(prhs[1]);

    stateLen = mxGetNumberOfElements(prhs[2]);

    if (stateLen == 0)
    {
        stateArray = mxCreateNumericMatrix(1, (mwSize)(numTaps + blockSize - 1), mxINT32_CLASS, mxREAL);
        pState = (int32_t *)mxGetData(stateArray);
        memset(pState, 0, (numTaps + blockSize - 1) * sizeof(int32_t));
    }
    else
    {
        stateArray = mxDuplicateArray(prhs[2]);
        pState = (int32_t *)mxGetData(stateArray);
    }

    S.numTaps = (uint16_t)numTaps;
    S.pCoeffs = pCoeffs;
    S.pState = pState;

    plhs[0] = mxCreateNumericMatrix(1, (mwSize)blockSize, mxINT32_CLASS, mxREAL);
    pDst = (int32_t *)mxGetData(plhs[0]);

    arm_fir_q31(&S, pSrc, pDst, (uint32_t)blockSize);

    if (nlhs >= 2)
    {
        plhs[1] = stateArray;
    }
    else
    {
        mxDestroyArray(stateArray);
    }
}