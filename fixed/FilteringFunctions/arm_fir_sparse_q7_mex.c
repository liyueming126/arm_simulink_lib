#include <stdint.h>
#include <string.h>
#include "mex.h"

typedef int8_t q7_t;
typedef int32_t q31_t;

typedef struct {
    uint16_t numTaps;
    uint16_t stateIndex;
    q7_t *pState;
    q7_t *pCoeffs;
    uint16_t maxDelay;
    int32_t *pTapDelay;
} arm_fir_sparse_instance_q7;

static int8_t ssat8(int32_t val)
{
    if (val > 127) return 127;
    if (val < -128) return -128;
    return (int8_t)val;
}

static void arm_circularWrite_q7(q7_t *pState, uint32_t delaySize, uint16_t *stateIndex,
    uint16_t srcInc, q7_t *pSrc, uint16_t dstInc, uint32_t blockSize)
{
    uint32_t idx = *stateIndex;
    uint32_t i;
    (void)dstInc;
    for (i = 0; i < blockSize; i++)
    {
        pState[idx] = *pSrc;
        pSrc += srcInc;
        idx++;
        if (idx >= delaySize) idx = 0;
    }
    *stateIndex = (uint16_t)idx;
}

static void arm_circularRead_q7(q7_t *pState, uint32_t delaySize, int32_t *readIndex,
    uint16_t srcInc, q7_t *pDst, q7_t *pDst2, uint32_t blockSize,
    uint16_t dstInc, uint32_t dstSize)
{
    int32_t idx = *readIndex;
    uint32_t i;
    (void)pDst2;
    (void)blockSize;
    (void)srcInc;
    for (i = 0; i < dstSize; i++)
    {
        *pDst = pState[idx];
        pDst += dstInc;
        idx++;
        if (idx >= (int32_t)delaySize) idx = 0;
    }
    *readIndex = idx;
}

static void arm_fir_sparse_q7(
    arm_fir_sparse_instance_q7 *S,
    q7_t *pSrc,
    q7_t *pDst,
    q7_t *pScratchIn,
    q31_t *pScratchOut,
    uint32_t blockSize)
{
    q7_t *pState = S->pState;
    q7_t *pIn = pSrc;
    q7_t *pOut = pDst;
    q7_t *pCoeffs = S->pCoeffs;
    q7_t *px;
    q7_t *pb = pScratchIn;
    q7_t *py = pState;
    int32_t *pTapDelay = S->pTapDelay;
    uint32_t delaySize = S->maxDelay + blockSize;
    uint16_t numTaps = S->numTaps;
    int32_t readIndex;
    uint32_t tapCnt, blkCnt;
    q7_t coeff = *pCoeffs++;
    q31_t *pScr2 = pScratchOut;

    arm_circularWrite_q7(py, delaySize, &S->stateIndex, 1, pIn, 1, blockSize);

    tapCnt = numTaps;

    readIndex = (S->stateIndex - blockSize) - *pTapDelay++;
    if (readIndex < 0)
    {
        readIndex += (int32_t)delaySize;
    }

    py = pState;
    arm_circularRead_q7(py, delaySize, &readIndex, 1, pb, pb, blockSize, 1, blockSize);

    px = pb;
    pScratchOut = pScr2;
    blkCnt = blockSize;

    while (blkCnt > 0u)
    {
        *pScratchOut++ = ((q31_t)*px++ * coeff);
        blkCnt--;
    }

    if (numTaps >= 2) {
        coeff = *pCoeffs++;
        readIndex = (S->stateIndex - blockSize) - *pTapDelay++;
        if (readIndex < 0)
        {
            readIndex += (int32_t)delaySize;
        }

        tapCnt = (uint32_t)numTaps - 2u;

        while (tapCnt > 0u)
        {
            py = pState;
            arm_circularRead_q7(py, delaySize, &readIndex, 1, pb, pb, blockSize, 1, blockSize);
            px = pb;
            pScratchOut = pScr2;
            blkCnt = blockSize;

            while (blkCnt > 0u)
            {
                *pScratchOut++ += (q31_t)*px++ * coeff;
                blkCnt--;
            }

            coeff = *pCoeffs++;
            readIndex = (S->stateIndex - blockSize) - *pTapDelay++;
            if (readIndex < 0)
            {
                readIndex += (int32_t)delaySize;
            }
            tapCnt--;
        }

        py = pState;
        arm_circularRead_q7(py, delaySize, &readIndex, 1, pb, pb, blockSize, 1, blockSize);
        px = pb;
        pScratchOut = pScr2;
        blkCnt = blockSize;

        while (blkCnt > 0u)
        {
            *pScratchOut++ += (q31_t)*px++ * coeff;
            blkCnt--;
        }
    }

    blkCnt = blockSize;
    while (blkCnt > 0u)
    {
        *pOut++ = ssat8(*pScr2++ >> 7);
        blkCnt--;
    }
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int8_t *pSrc, *pCoeffs, *pState, *pDst, *pScratchIn;
    int32_t *pTapDelay, *pScratchOut;
    uint32_t blockSize;
    uint16_t numTaps, maxDelay, stateIndex;
    arm_fir_sparse_instance_q7 S;
    mxArray *stateArray;
    uint32_t i;

    if (nrhs < 3 || nrhs > 4)
        mexErrMsgIdAndTxt("arm_fir_sparse_q7_mex:invalidInput",
            "Inputs: x(int8), coeffs(int8), tapDelay(int32), [state(int8 empty)]");

    if (!mxIsInt8(prhs[0]) || mxIsComplex(prhs[0]))
        mexErrMsgIdAndTxt("arm_fir_sparse_q7_mex:invalidType","Input 1 (x) must be int8.");
    if (!mxIsInt8(prhs[1]) || mxIsComplex(prhs[1]))
        mexErrMsgIdAndTxt("arm_fir_sparse_q7_mex:invalidType","Input 2 (coeffs) must be int8.");
    if (!mxIsInt32(prhs[2]) || mxIsComplex(prhs[2]))
        mexErrMsgIdAndTxt("arm_fir_sparse_q7_mex:invalidType","Input 3 (tapDelay) must be int32.");

    pSrc = (int8_t *)mxGetData(prhs[0]);
    blockSize = (uint32_t)mxGetNumberOfElements(prhs[0]);

    pCoeffs = (int8_t *)mxGetData(prhs[1]);
    numTaps = (uint16_t)mxGetNumberOfElements(prhs[1]);

    pTapDelay = (int32_t *)mxGetData(prhs[2]);

    maxDelay = 0;
    for (i = 0; i < mxGetNumberOfElements(prhs[2]); i++)
        if ((uint16_t)pTapDelay[i] > maxDelay) maxDelay = (uint16_t)pTapDelay[i];

    if (nrhs >= 4 && mxIsInt8(prhs[3]) && mxGetNumberOfElements(prhs[3]) > 0)
        stateArray = mxDuplicateArray(prhs[3]);
    else
    {
        stateArray = mxCreateNumericMatrix(1, (mwSize)(maxDelay + blockSize), mxINT8_CLASS, mxREAL);
        memset(mxGetData(stateArray), 0, (maxDelay + blockSize) * sizeof(int8_t));
    }

    pState = (int8_t *)mxGetData(stateArray);
    stateIndex = 0;

    pScratchIn = (int8_t *)mxCalloc(blockSize, sizeof(int8_t));
    pScratchOut = (int32_t *)mxCalloc(blockSize, sizeof(int32_t));

    S.numTaps = numTaps;
    S.pCoeffs = pCoeffs;
    S.pState = pState;
    S.maxDelay = maxDelay;
    S.pTapDelay = pTapDelay;
    S.stateIndex = stateIndex;

    plhs[0] = mxCreateNumericMatrix(1, (mwSize)blockSize, mxINT8_CLASS, mxREAL);
    pDst = (int8_t *)mxGetData(plhs[0]);

    arm_fir_sparse_q7(&S, pSrc, pDst, pScratchIn, pScratchOut, blockSize);

    if (nlhs >= 2) plhs[1] = stateArray;
    else mxDestroyArray(stateArray);

    mxFree(pScratchIn);
    mxFree(pScratchOut);
}