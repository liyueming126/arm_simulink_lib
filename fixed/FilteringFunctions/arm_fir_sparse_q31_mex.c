#include <stdint.h>
#include <string.h>
#include "mex.h"

typedef int16_t q15_t;
typedef int32_t q31_t;
typedef int64_t q63_t;

typedef struct {
    uint16_t numTaps;
    uint16_t stateIndex;
    q31_t *pState;
    q31_t *pCoeffs;
    uint16_t maxDelay;
    int32_t *pTapDelay;
} arm_fir_sparse_instance_q31;

static void arm_circularWrite_q31(q31_t *pState, uint32_t delaySize, uint16_t *stateIndex,
    uint16_t srcInc, q31_t *pSrc, uint16_t dstInc, uint32_t blockSize)
{
    uint32_t idx = *stateIndex;
    uint32_t i;
    for (i = 0; i < blockSize; i++) {
        pState[idx] = *pSrc; pSrc += srcInc; idx++;
        if (idx >= delaySize) idx = 0;
    }
    *stateIndex = (uint16_t)idx;
    (void)dstInc;
}

static void arm_circularRead_q31(q31_t *pState, uint32_t delaySize, int32_t *readIndex,
    uint16_t srcInc, q31_t *pDst, q31_t *pDst2, uint32_t blockSize,
    uint16_t dstInc, uint32_t dstSize)
{
    int32_t idx = *readIndex;
    uint32_t i;
    (void)pDst2; (void)blockSize;
    for (i = 0; i < dstSize; i++) {
        *pDst = pState[idx]; pDst += dstInc; idx++;
        if (idx >= (int32_t)delaySize) idx = 0;
    }
    *readIndex = idx;
    (void)srcInc;
}

static void arm_fir_sparse_q31(
    arm_fir_sparse_instance_q31 *S,
    q31_t *pSrc, q31_t *pDst, q31_t *pScratchIn, uint32_t blockSize)
{
    q31_t *pState = S->pState;
    q31_t *pCoeffs = S->pCoeffs;
    q31_t *px, *py = pState, *pb = pScratchIn, *pOut;
    q63_t out;
    int32_t *pTapDelay = S->pTapDelay;
    uint32_t delaySize = S->maxDelay + blockSize;
    uint16_t numTaps = S->numTaps;
    int32_t readIndex;
    uint32_t tapCnt, blkCnt;
    q31_t coeff = *pCoeffs++;
    q31_t in;

    arm_circularWrite_q31(py, delaySize, &S->stateIndex, 1, pSrc, 1, blockSize);
    readIndex = (int32_t)(S->stateIndex - blockSize) - *pTapDelay++;
    if (readIndex < 0) readIndex += (int32_t)delaySize;
    py = pState;
    arm_circularRead_q31(py, delaySize, &readIndex, 1, pb, pb, blockSize, 1, blockSize);
    px = pb; pOut = pDst; blkCnt = blockSize;
    while (blkCnt > 0u) { *pOut++ = (q31_t)(((q63_t)*px++ * coeff) >> 32); blkCnt--; }
    if (numTaps >= 2) {
        coeff = *pCoeffs++;
        readIndex = (int32_t)(S->stateIndex - blockSize) - *pTapDelay++;
        if (readIndex < 0) readIndex += (int32_t)delaySize;
        tapCnt = (uint32_t)numTaps - 2u;
        while (tapCnt > 0u) {
            py = pState;
            arm_circularRead_q31(py, delaySize, &readIndex, 1, pb, pb, blockSize, 1, blockSize);
            px = pb; pOut = pDst; blkCnt = blockSize;
            while (blkCnt > 0u) { out = *pOut; out += ((q63_t)*px++ * coeff) >> 32; *pOut++ = (q31_t)(out); blkCnt--; }
            coeff = *pCoeffs++;
            readIndex = (int32_t)(S->stateIndex - blockSize) - *pTapDelay++;
            if (readIndex < 0) readIndex += (int32_t)delaySize;
            tapCnt--;
        }
        py = pState;
        arm_circularRead_q31(py, delaySize, &readIndex, 1, pb, pb, blockSize, 1, blockSize);
        px = pb; pOut = pDst; blkCnt = blockSize;
        while (blkCnt > 0u) { out = *pOut; out += ((q63_t)*px++ * coeff) >> 32; *pOut++ = (q31_t)(out); blkCnt--; }
    }
    pOut = pDst; blkCnt = blockSize;
    while (blkCnt > 0u) { in = *pOut << 1; *pOut++ = in; blkCnt--; }
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int32_t *pSrc, *pCoeffs, *pState, *pDst, *pScratchIn;
    int32_t *pTapDelay;
    uint32_t blockSize;
    uint16_t numTaps, maxDelay, stateIndex;
    arm_fir_sparse_instance_q31 S;
    mxArray *stateArray;
    uint32_t i;

    if (nrhs < 3 || nrhs > 4)
        mexErrMsgIdAndTxt("arm_fir_sparse_q31_mex:invalidInput",
            "Inputs: x(int32), coeffs(int32), tapDelay(int32), [state(int32 empty)]");

    if (!mxIsInt32(prhs[0]) || mxIsComplex(prhs[0]))
        mexErrMsgIdAndTxt("arm_fir_sparse_q31_mex:invalidType","Input 1 (x) must be int32.");
    if (!mxIsInt32(prhs[1]) || mxIsComplex(prhs[1]))
        mexErrMsgIdAndTxt("arm_fir_sparse_q31_mex:invalidType","Input 2 (coeffs) must be int32.");
    if (!mxIsInt32(prhs[2]) || mxIsComplex(prhs[2]))
        mexErrMsgIdAndTxt("arm_fir_sparse_q31_mex:invalidType","Input 3 (tapDelay) must be int32.");

    pSrc = (int32_t *)mxGetData(prhs[0]);
    blockSize = (uint32_t)mxGetNumberOfElements(prhs[0]);
    pCoeffs = (int32_t *)mxGetData(prhs[1]);
    numTaps = (uint16_t)mxGetNumberOfElements(prhs[1]);
    pTapDelay = (int32_t *)mxGetData(prhs[2]);

    maxDelay = 0;
    for (i = 0; i < mxGetNumberOfElements(prhs[2]); i++)
        if ((uint16_t)pTapDelay[i] > maxDelay) maxDelay = (uint16_t)pTapDelay[i];

    if (nrhs >= 4 && mxIsInt32(prhs[3]) && mxGetNumberOfElements(prhs[3]) > 0)
        stateArray = mxDuplicateArray(prhs[3]);
    else {
        stateArray = mxCreateNumericMatrix(1, (mwSize)(maxDelay + blockSize), mxINT32_CLASS, mxREAL);
        memset(mxGetData(stateArray), 0, (maxDelay + blockSize) * sizeof(int32_t));
    }

    pState = (int32_t *)mxGetData(stateArray);
    stateIndex = 0;
    pScratchIn = (int32_t *)mxCalloc(blockSize, sizeof(int32_t));

    S.numTaps = numTaps;
    S.pCoeffs = pCoeffs;
    S.pState = pState;
    S.maxDelay = maxDelay;
    S.pTapDelay = pTapDelay;
    S.stateIndex = stateIndex;

    plhs[0] = mxCreateNumericMatrix(1, (mwSize)blockSize, mxINT32_CLASS, mxREAL);
    pDst = (int32_t *)mxGetData(plhs[0]);

    arm_fir_sparse_q31(&S, pSrc, pDst, pScratchIn, blockSize);

    if (nlhs >= 2) plhs[1] = stateArray;
    else mxDestroyArray(stateArray);

    mxFree(pScratchIn);
}