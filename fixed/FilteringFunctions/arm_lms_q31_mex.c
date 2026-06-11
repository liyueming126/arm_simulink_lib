#include <stdint.h>
#include <string.h>
#include "mex.h"

typedef int16_t q15_t;
typedef int32_t q31_t;
typedef int64_t q63_t;

typedef struct {
    uint16_t numTaps;
    q31_t *pState;
    q31_t *pCoeffs;
    q31_t mu;
    uint32_t postShift;
} arm_lms_instance_q31;

static int32_t clip_q63_to_q31(int64_t x)
{
    if (x > 2147483647LL) return 2147483647L;
    if (x < -2147483648LL) return -2147483648L;
    return (int32_t)x;
}

static void arm_lms_q31(
    const arm_lms_instance_q31 *S,
    q31_t *pSrc, q31_t *pRef, q31_t *pOut, q31_t *pErr,
    uint32_t blockSize)
{
    q31_t *pState = S->pState;
    uint32_t numTaps = S->numTaps;
    q31_t *pCoeffs = S->pCoeffs;
    q31_t *pStateCurnt;
    q31_t mu = S->mu;
    q31_t *px, *pb;
    uint32_t tapCnt, blkCnt;
    q63_t acc;
    q31_t e = 0, alpha, coef;
    q31_t acc_l, acc_h;
    uint32_t uShift = ((uint32_t)S->postShift + 1u);
    uint32_t lShift = 32u - uShift;

    pStateCurnt = &(S->pState[(numTaps - 1u)]);
    blkCnt = blockSize;

    while (blkCnt > 0u)
    {
        *pStateCurnt++ = *pSrc++;
        px = pState; pb = pCoeffs; acc = 0; tapCnt = numTaps;
        while (tapCnt > 0u) { acc += ((q63_t)(*px++)) * (*pb++); tapCnt--; }
        acc_l = acc & 0xffffffff;
        acc_h = (acc >> 32) & 0xffffffff;
        acc = (uint32_t)acc_l >> lShift | acc_h << uShift;
        *pOut++ = (q31_t)acc;
        e = *pRef++ - (q31_t)acc; *pErr++ = (q31_t)e;
        alpha = (q31_t)(((q63_t)e * mu) >> 31);
        px = pState++; pb = pCoeffs; tapCnt = numTaps;
        while (tapCnt > 0u) { coef = (q31_t)(((q63_t)alpha * (*px++)) >> (32)); *pb = clip_q63_to_q31((q63_t)*pb + (coef << 1u)); pb++; tapCnt--; }
        blkCnt--;
    }

    pStateCurnt = S->pState;
    tapCnt = (numTaps - 1u);
    while (tapCnt > 0u) { *pStateCurnt++ = *pState++; tapCnt--; }
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int32_t *pSrc, *pRef, *pCoeffs, *pState, *pOut, *pErr;
    uint32_t blockSize;
    uint16_t numTaps;
    int32_t muVal;
    uint32_t postShiftVal;
    arm_lms_instance_q31 S;
    mxArray *stateArray, *coeffArray;

    if (nrhs < 3 || nrhs > 5)
        mexErrMsgIdAndTxt("arm_lms_q31_mex:invalidInput",
            "Inputs: x(int32), d(int32), coeffs(int32), [mu(int32)=0x40000000], [postShift(uint32)=0]");

    if (!mxIsInt32(prhs[0]) || mxIsComplex(prhs[0]))
        mexErrMsgIdAndTxt("arm_lms_q31_mex:invalidType","Input 1 (x) must be int32.");
    if (!mxIsInt32(prhs[1]) || mxIsComplex(prhs[1]))
        mexErrMsgIdAndTxt("arm_lms_q31_mex:invalidType","Input 2 (d) must be int32.");
    if (!mxIsInt32(prhs[2]) || mxIsComplex(prhs[2]))
        mexErrMsgIdAndTxt("arm_lms_q31_mex:invalidType","Input 3 (coeffs) must be int32.");

    pSrc = (int32_t *)mxGetData(prhs[0]);
    blockSize = (uint32_t)mxGetNumberOfElements(prhs[0]);

    pRef = (int32_t *)mxGetData(prhs[1]);
    if (mxGetNumberOfElements(prhs[1]) != (size_t)blockSize)
        mexErrMsgIdAndTxt("arm_lms_q31_mex:invalidSize","x and d must have same length.");

    coeffArray = mxDuplicateArray(prhs[2]);
    pCoeffs = (int32_t *)mxGetData(coeffArray);
    numTaps = (uint16_t)mxGetNumberOfElements(prhs[2]);

    stateArray = mxCreateNumericMatrix(1, (mwSize)(numTaps + blockSize - 1), mxINT32_CLASS, mxREAL);
    pState = (int32_t *)mxGetData(stateArray);
    memset(pState, 0, (numTaps + blockSize - 1) * sizeof(int32_t));

    muVal = 0x40000000;
    if (nrhs >= 4 && mxIsInt32(prhs[3]) && !mxIsEmpty(prhs[3]))
        muVal = ((int32_t *)mxGetData(prhs[3]))[0];

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

    plhs[0] = mxCreateNumericMatrix(1, (mwSize)blockSize, mxINT32_CLASS, mxREAL);
    pOut = (int32_t *)mxGetData(plhs[0]);
    plhs[1] = mxCreateNumericMatrix(1, (mwSize)blockSize, mxINT32_CLASS, mxREAL);
    pErr = (int32_t *)mxGetData(plhs[1]);

    arm_lms_q31(&S, pSrc, pRef, pOut, pErr, blockSize);

    if (nlhs >= 3) { plhs[2] = coeffArray; }
    else { mxDestroyArray(coeffArray); }

    mxDestroyArray(stateArray);
}