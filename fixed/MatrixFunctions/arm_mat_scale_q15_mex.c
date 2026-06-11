#include <stdint.h>
#include "mex.h"

typedef int16_t q15_t;
typedef int32_t arm_status;

#define ARM_MATH_SUCCESS       0
#define ARM_MATH_SIZE_MISMATCH 1

typedef struct {
    uint16_t numRows;
    uint16_t numCols;
    q15_t *pData;
} arm_matrix_instance_q15;

arm_status arm_mat_scale_q15(
    const arm_matrix_instance_q15 *pSrc,
    q15_t scaleFract,
    int32_t shift,
    arm_matrix_instance_q15 *pDst)
{
    q15_t *pIn = pSrc->pData;
    q15_t *pOut = pDst->pData;
    uint32_t numSamples;
    uint32_t blkCnt;
    int32_t val;
    int32_t totShift;

    if ((pSrc->numRows != pDst->numRows) || (pSrc->numCols != pDst->numCols))
        return ARM_MATH_SIZE_MISMATCH;

    numSamples = (uint32_t)pSrc->numRows * pSrc->numCols;
    totShift = 15 - shift;
    blkCnt = numSamples;

    while (blkCnt > 0u)
    {
        val = ((int32_t)*pIn++ * (int32_t)scaleFract) >> totShift;

        if (val > 32767)
            val = 32767;
        else if (val < -32768)
            val = -32768;

        *pOut++ = (q15_t)val;
        blkCnt--;
    }

    return ARM_MATH_SUCCESS;
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int16_t *pSrcData, *pDstData;
    int16_t scaleFract;
    int32_t shift;
    size_t rows, cols;
    arm_matrix_instance_q15 src, dst;

    if (nrhs != 3)
        mexErrMsgIdAndTxt("arm_mat_scale_q15:invalidInput", "3 inputs required (matrix, scaleFract, shift).");
    if (nlhs > 1)
        mexErrMsgIdAndTxt("arm_mat_scale_q15:invalidOutput", "At most 1 output allowed.");
    if (!mxIsInt16(prhs[0]) || mxIsComplex(prhs[0]) || mxIsSparse(prhs[0]))
        mexErrMsgIdAndTxt("arm_mat_scale_q15:invalidInputType", "Input 1 must be real int16 matrix.");
    if (!mxIsInt16(prhs[1]) || mxIsComplex(prhs[1]) || mxGetNumberOfElements(prhs[1]) != 1)
        mexErrMsgIdAndTxt("arm_mat_scale_q15:invalidInputType", "Input 2 (scaleFract) must be a real int16 scalar.");
    if (!mxIsInt32(prhs[2]) || mxIsComplex(prhs[2]) || mxGetNumberOfElements(prhs[2]) != 1)
        mexErrMsgIdAndTxt("arm_mat_scale_q15:invalidInputType", "Input 3 (shift) must be a real int32 scalar.");

    rows = mxGetM(prhs[0]);
    cols = mxGetN(prhs[0]);
    pSrcData = (int16_t *)mxGetData(prhs[0]);
    scaleFract = *(int16_t *)mxGetData(prhs[1]);
    shift = *(int32_t *)mxGetData(prhs[2]);

    plhs[0] = mxCreateNumericMatrix(rows, cols, mxINT16_CLASS, mxREAL);
    pDstData = (int16_t *)mxGetData(plhs[0]);

    src.numRows = (uint16_t)rows;
    src.numCols = (uint16_t)cols;
    src.pData = pSrcData;

    dst.numRows = (uint16_t)rows;
    dst.numCols = (uint16_t)cols;
    dst.pData = pDstData;

    arm_mat_scale_q15(&src, scaleFract, shift, &dst);
}