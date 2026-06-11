/* ----------------------------------------------------------------------
 * arm_dct4_q31_mex.c
 * MEX wrapper for arm_dct4_q31.
 * Includes the self-contained simulink implementation.
 *
 * Usage:
 *   y = arm_dct4_q31_mex(S, pInlineBuffer)
 *   S: structure with DCT4 instance parameters
 *   pInlineBuffer: int32 vector of length N (in-place)
 *
 * The MEX function creates pState internally (length 2*N for complex RFFT output).
 * -------------------------------------------------------------------- */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "mex.h"

/* Include the self-contained DCT4 implementation */
#include "arm_dct4_q31_simulink.c"

/* RFFT implementation needed by DCT4 */
#include "arm_rfft_q31_simulink.c"
#include "arm_cfft_q31_simulink.c"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    arm_dct4_instance_q31 S;
    q31_t *pInlineBuffer;
    q31_t *pState;
    size_t dataLen;
    uint32_t N;

    /* Check inputs */
    if (nrhs < 2 || nrhs > 2)
        mexErrMsgIdAndTxt("arm_dct4_q31_mex:invalidInput",
            "Usage: y = arm_dct4_q31_mex(S, pInlineBuffer)");
    if (nlhs > 1)
        mexErrMsgIdAndTxt("arm_dct4_q31_mex:invalidOutput",
            "At most 1 output allowed.");

    /* First argument: struct S */
    if (!mxIsStruct(prhs[0]))
        mexErrMsgIdAndTxt("arm_dct4_q31_mex:invalidStruct",
            "First argument must be a struct.");

    /* Get N from struct field */
    mxArray *fN = mxGetField(prhs[0], 0, "N");
    if (!fN)
        mexErrMsgIdAndTxt("arm_dct4_q31_mex:missingField",
            "Struct must have field 'N'.");
    S.N = (uint16_t)mxGetScalar(fN);

    mxArray *fNby2 = mxGetField(prhs[0], 0, "Nby2");
    if (!fNby2)
        mexErrMsgIdAndTxt("arm_dct4_q31_mex:missingField",
            "Struct must have field 'Nby2'.");
    S.Nby2 = (uint16_t)mxGetScalar(fNby2);

    mxArray *fNorm = mxGetField(prhs[0], 0, "normalize");
    if (!fNorm)
        mexErrMsgIdAndTxt("arm_dct4_q31_mex:missingField",
            "Struct must have field 'normalize'.");
    S.normalize = (q31_t)mxGetScalar(fNorm);

    mxArray *fTwiddle = mxGetField(prhs[0], 0, "pTwiddle");
    if (!fTwiddle || !mxIsInt32(fTwiddle))
        mexErrMsgIdAndTxt("arm_dct4_q31_mex:missingField",
            "Struct must have field 'pTwiddle' (int32).");
    S.pTwiddle = (q31_t *)mxGetData(fTwiddle);

    mxArray *fCosFact = mxGetField(prhs[0], 0, "pCosFactor");
    if (!fCosFact || !mxIsInt32(fCosFact))
        mexErrMsgIdAndTxt("arm_dct4_q31_mex:missingField",
            "Struct must have field 'pCosFactor' (int32).");
    S.pCosFactor = (q31_t *)mxGetData(fCosFact);

    mxArray *fpRfft = mxGetField(prhs[0], 0, "pRfft");
    if (!fpRfft)
        mexErrMsgIdAndTxt("arm_dct4_q31_mex:missingField",
            "Struct must have field 'pRfft'.");
    /* pRfft is void*, store the pointer value from the MATLAB field (uint64) */
    if (mxIsUint64(fpRfft))
    {
        uint64_t *ptrVal = (uint64_t *)mxGetData(fpRfft);
        S.pRfft = (void *)(*ptrVal);
    }
    else
    {
        mexErrMsgIdAndTxt("arm_dct4_q31_mex:invalidRfft",
            "pRfft must be uint64 holding a pointer.");
    }

    /* Second argument: input buffer */
    if (!mxIsInt32(prhs[1]) || mxIsComplex(prhs[1]))
        mexErrMsgIdAndTxt("arm_dct4_q31_mex:invalidInputType",
            "Input must be real int32.");

    dataLen = mxGetNumberOfElements(prhs[1]);
    if (dataLen != S.N)
        mexErrMsgIdAndTxt("arm_dct4_q31_mex:invalidDim",
            "Input length must equal S.N (%d).", S.N);

    N = S.N;

    /* Create output (in-place) */
    plhs[0] = mxCreateNumericMatrix(1, (mwSize)N, mxINT32_CLASS, mxREAL);
    memcpy(mxGetData(plhs[0]), mxGetData(prhs[1]), N * sizeof(int32_t));
    pInlineBuffer = (q31_t *)mxGetData(plhs[0]);

    /* Allocate pState buffer (length 2*N for complex RFFT output) */
    pState = (q31_t *)mxCalloc(2 * N, sizeof(q31_t));
    if (!pState)
        mexErrMsgIdAndTxt("arm_dct4_q31_mex:alloc",
            "Failed to allocate pState buffer.");

    /* Run DCT4 */
    arm_dct4_q31(&S, pState, pInlineBuffer);

    mxFree(pState);
}