#include <stdint.h>
#include <string.h>
#include "mex.h"

typedef int16_t q15_t;
typedef int32_t q31_t;
typedef int64_t q63_t;

typedef struct {
    uint16_t numTaps;
    uint16_t stateIndex;
    q15_t *pState;
    q15_t *pCoeffs;
    uint16_t maxDelay;
    int32_t *pTapDelay;
} arm_fir_sparse_instance_q15;

static int16_t ssat16(int32_t val)
{
    if (val > 32767) return 32767;
    if (val < -32768) return -32768;
    return (int16_t)val;
}

static void arm_circularWrite_q15(q15_t *pState, uint32_t delaySize, uint16_t *stateIndex,
    uint16_t srcInc, q15_t *pSrc, uint16_t dstInc, uint32_t blockSize)
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

static void arm_circularRead_q15(q15_t *pState, uint32_t delaySize, int32_t *readIndex,
    uint16_t srcInc, q15_t *pDst, q15_t *pDst2, uint32_t blockSize,
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

static void arm_fir_sparse_q15(
    arm_fir_sparse_instance_q15 *S,
    q15_t *pSrc, q15_t *pDst, q15_t *pScratchIn, q31_t *pScratchOut,
    uint32_t blockSize)
{
    q15_t *pState = S->pState;
    q15_t *pIn = pSrc;
    q15_t *pOut = pDst;
    q15_t *pCoeffs = S->pCoeffs;
    q15_t *px, *pb = pScratchIn;
    q15_t *py = pState;
    int32_t *pTapDelay = S->pTapDelay;
    uint32_t delaySize = S->maxDelay + blockSize;
    uint16_t numTaps = S->numTaps;
    int32_t readIndex;
    uint32_t tapCnt, blkCnt;
    q15_t coeff = *pCoeffs++;
    q31_t *pScr2 = pScratchOut;

    arm_circularWrite_q15(py, delaySize, &S->stateIndex, 1, pIn, 1, blockSize);
    tapCnt = numTaps;
    readIndex = (S->stateIndex - blockSize) - *pTapDelay++;
    if (readIndex < 0) readIndex += (int32_t)delaySize;
    py = pState;
    arm_circularRead_q15(py, delaySize, &readIndex, 1, pb, pb, blockSize, 1, blockSize);
    px = pb; pScratchOut = pScr2; blkCnt = blockSize;
    while (blkCnt > 0u) { *pScratchOut++ = ((q31_t)*px++ * coeff); blkCnt--; }
    if (numTaps >= 2) {
        coeff = *pCoeffs++;
        readIndex = (S->stateIndex - blockSize) - *pTapDelay++;
        if (readIndex < 0) readIndex += (int32_t)delaySize;
        tapCnt = (uint32_t)numTaps - 2u;
        while (tapCnt > 0u) {
            py = pState;
            arm_circularRead_q15(py, delaySize, &readIndex, 1, pb, pb, blockSize, 1, blockSize);
            px = pb; pScratchOut = pScr2; blkCnt = blockSize;
            while (blkCnt > 0u) { *pScratchOut++ += (q31_t)*px++ * coeff; blkCnt--; }
            coeff = *pCoeffs++;
            readIndex = (S->stateIndex - blockSize) - *pTapDelay++;
            if (readIndex < 0) readIndex += (int32_t)delaySize;
            tapCnt--;
        }
        py = pState;
        arm_circularRead_q15(py, delaySize, &readIndex, 1, pb, pb, blockSize, 1, blockSize);
        px = pb; pScratchOut = pScr2; blkCnt = blockSize;
        while (blkCnt > 0u) { *pScratchOut++ += (q31_t)*px++ * coeff; blkCnt--; }
    }
    blkCnt = blockSize;
    while (blkCnt > 0u) { *pOut++ = ssat16(*pScr2++ >> 15); blkCnt--; }
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int16_t *pSrc, *pCoeffs, *pState, *pDst, *pScratchIn;
    int32_t *pTapDelay, *pScratchOut;
    uint32_t blockSize;
    uint16_t numTaps, maxDelay, stateIndex;
    arm_fir_sparse_instance_q15 S;
    mxArray *stateArray;
    size_t numTaps_size, maxDelay_size;
    uint32_t i;

    if (nrhs < 3 || nrhs > 4)
        mexErrMsgIdAndTxt("arm_fir_sparse_q15_mex:invalidInput",
            "Inputs: x(int16), coeffs(int16), tapDelay(int32), [state(int16 empty)]");

    if (!mxIsInt16(prhs[0]) || mxIsComplex(prhs[0]))
        mexErrMsgIdAndTxt("arm_fir_sparse_q15_mex:invalidType","Input 1 (x) must be int16.");
    if (!mxIsInt16(prhs[1]) || mxIsComplex(prhs[1]))
        mexErrMsgIdAndTxt("arm_fir_sparse_q15_mex:invalidType","Input 2 (coeffs) must be int16.");
    if (!mxIsInt32(prhs[2]) || mxIsComplex(prhs[2]))
        mexErrMsgIdAndTxt("arm_fir_sparse_q15_mex:invalidType","Input 3 (tapDelay) must be int32.");

    pSrc = (int16_t *)mxGetData(prhs[0]);
    blockSize = (uint32_t)mxGetNumberOfElements(prhs[0]);

    pCoeffs = (int16_t *)mxGetData(prhs[1]);
    numTaps = (uint16_t)mxGetNumberOfElements(prhs[1]);
    numTaps_size = numTaps;

    pTapDelay = (int32_t *)mxGetData(prhs[2]);

    maxDelay = 0;
    for (i = 0; i < mxGetNumberOfElements(prhs[2]); i++)
        if ((uint16_t)pTapDelay[i] > maxDelay) maxDelay = (uint16_t)pTapDelay[i];
    maxDelay_size = maxDelay;

    if (nrhs >= 4 && mxIsInt16(prhs[3]) && mxGetNumberOfElements(prhs[3]) > 0)
        stateArray = mxDuplicateArray(prhs[3]);
    else {
        stateArray = mxCreateNumericMatrix(1, (mwSize)(maxDelay + blockSize), mxINT16_CLASS, mxREAL);
        memset(mxGetData(stateArray), 0, (maxDelay + blockSize) * sizeof(int16_t));
    }

    pState = (int16_t *)mxGetData(stateArray);
    stateIndex = 0;

    pScratchIn = (int16_t *)mxCalloc(blockSize, sizeof(int16_t));
    pScratchOut = (int32_t *)mxCalloc(blockSize, sizeof(int32_t));

    S.numTaps = numTaps;
    S.pCoeffs = pCoeffs;
    S.pState = pState;
    S.maxDelay = maxDelay;
    S.pTapDelay = pTapDelay;
    S.stateIndex = stateIndex;

    plhs[0] = mxCreateNumericMatrix(1, (mwSize)blockSize, mxINT16_CLASS, mxREAL);
    pDst = (int16_t *)mxGetData(plhs[0]);

    arm_fir_sparse_q15(&S, pSrc, pDst, pScratchIn, pScratchOut, blockSize);

    if (nlhs >= 2) plhs[1] = stateArray;
    else mxDestroyArray(stateArray);

    mxFree(pScratchIn);
    mxFree(pScratchOut);
}