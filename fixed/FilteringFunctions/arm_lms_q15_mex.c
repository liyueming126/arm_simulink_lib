#include <stdint.h>
#include <string.h>
#include "mex.h"

typedef int16_t q15_t;
typedef int32_t q31_t;
typedef int64_t q63_t;

typedef struct {
    uint16_t numTaps;
    q15_t *pState;
    q15_t *pCoeffs;
    q15_t mu;
    uint32_t postShift;
} arm_lms_instance_q15;

static int16_t ssat16(int32_t val)
{
    if (val > 32767) return 32767;
    if (val < -32768) return -32768;
    return (int16_t)val;
}

static void arm_lms_q15(
    const arm_lms_instance_q15 *S,
    q15_t *pSrc, q15_t *pRef, q15_t *pOut, q15_t *pErr,
    uint32_t blockSize)
{
    q15_t *pState = S->pState;
    uint32_t numTaps = S->numTaps;
    q15_t *pCoeffs = S->pCoeffs;
    q15_t *pStateCurnt;
    q15_t mu = S->mu;
    q15_t *px, *pb;
    uint32_t tapCnt, blkCnt;
    q63_t acc;
    q15_t e = 0;
    q15_t alpha;
    q31_t coef;
    q31_t acc_l, acc_h;
    int32_t lShift = (15 - (int32_t)S->postShift);
    int32_t uShift = (32 - lShift);

    pStateCurnt = &(S->pState[(numTaps - 1u)]);
    blkCnt = blockSize;

    while (blkCnt > 0u)
    {
        *pStateCurnt++ = *pSrc++;
        px = pState; pb = pCoeffs; acc = 0; tapCnt = numTaps;
        while (tapCnt > 0u) { acc += (q63_t)((q31_t)(*px++) * (*pb++)); tapCnt--; }
        acc_l = acc & 0xffffffff;
        acc_h = (acc >> 32) & 0xffffffff;
        acc = (uint32_t)acc_l >> lShift | acc_h << uShift;
        acc = ssat16((int32_t)acc);
        *pOut++ = (q15_t)acc;
        e = *pRef++ - (q15_t)acc; *pErr++ = (q15_t)e;
        alpha = (q15_t)(((q31_t)e * (mu)) >> 15);
        px = pState++; pb = pCoeffs; tapCnt = numTaps;
        while (tapCnt > 0u) { coef = (q31_t)*pb + (((q31_t)alpha * (*px++)) >> 15); *pb++ = ssat16(coef); tapCnt--; }
        blkCnt--;
    }

    pStateCurnt = S->pState;
    tapCnt = (numTaps - 1u);
    while (tapCnt > 0u) { *pStateCurnt++ = *pState++; tapCnt--; }
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int16_t *pSrc, *pRef, *pCoeffs, *pState, *pOut, *pErr;
    uint32_t blockSize;
    uint16_t numTaps;
    int16_t muVal;
    uint32_t postShiftVal;
    arm_lms_instance_q15 S;
    mxArray *stateArray, *coeffArray;

    if (nrhs < 3 || nrhs > 5)
        mexErrMsgIdAndTxt("arm_lms_q15_mex:invalidInput",
            "Inputs: x(int16), d(int16), coeffs(int16), [mu(int16)=512], [postShift(uint32)=0]");

    if (!mxIsInt16(prhs[0]) || mxIsComplex(prhs[0]))
        mexErrMsgIdAndTxt("arm_lms_q15_mex:invalidType","Input 1 (x) must be int16.");
    if (!mxIsInt16(prhs[1]) || mxIsComplex(prhs[1]))
        mexErrMsgIdAndTxt("arm_lms_q15_mex:invalidType","Input 2 (d) must be int16.");
    if (!mxIsInt16(prhs[2]) || mxIsComplex(prhs[2]))
        mexErrMsgIdAndTxt("arm_lms_q15_mex:invalidType","Input 3 (coeffs) must be int16.");

    pSrc = (int16_t *)mxGetData(prhs[0]);
    blockSize = (uint32_t)mxGetNumberOfElements(prhs[0]);

    pRef = (int16_t *)mxGetData(prhs[1]);
    if (mxGetNumberOfElements(prhs[1]) != (size_t)blockSize)
        mexErrMsgIdAndTxt("arm_lms_q15_mex:invalidSize","x and d must have same length.");

    coeffArray = mxDuplicateArray(prhs[2]);
    pCoeffs = (int16_t *)mxGetData(coeffArray);
    numTaps = (uint16_t)mxGetNumberOfElements(prhs[2]);

    stateArray = mxCreateNumericMatrix(1, (mwSize)(numTaps + blockSize - 1), mxINT16_CLASS, mxREAL);
    pState = (int16_t *)mxGetData(stateArray);
    memset(pState, 0, (numTaps + blockSize - 1) * sizeof(int16_t));

    muVal = 512;
    if (nrhs >= 4 && mxIsInt16(prhs[3]) && !mxIsEmpty(prhs[3]))
        muVal = ((int16_t *)mxGetData(prhs[3]))[0];

    postShiftVal = 0;
    if (nrhs >= 5 && !mxIsEmpty(prhs[4]))
    {
        if (mxIsDouble(prhs[4])) postShiftVal = (uint32_t)((double *)mxGetData(prhs[4]))[0];
        else if (mxIsInt32(prhs[4])) postShiftVal = (uint32_t)((int32_t *)mxGetData(prhs[4]))[0];
    }

    S.numTaps = numTaps;
    S.pCoeffs = pCoeffs;
    S.pState = pState;
    S.mu = muVal;
    S.postShift = postShiftVal;

    plhs[0] = mxCreateNumericMatrix(1, (mwSize)blockSize, mxINT16_CLASS, mxREAL);
    pOut = (int16_t *)mxGetData(plhs[0]);

    plhs[1] = mxCreateNumericMatrix(1, (mwSize)blockSize, mxINT16_CLASS, mxREAL);
    pErr = (int16_t *)mxGetData(plhs[1]);

    arm_lms_q15(&S, pSrc, pRef, pOut, pErr, blockSize);

    if (nlhs >= 3)
    {
        plhs[2] = coeffArray;
    }
    else
    {
        mxDestroyArray(coeffArray);
    }

    mxDestroyArray(stateArray);
}