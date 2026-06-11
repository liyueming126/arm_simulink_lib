#include <stdint.h>
#include "mex.h"

typedef int32_t q31_t;
typedef int64_t q63_t;
typedef int32_t arm_status;

#define ARM_MATH_SUCCESS       0
#define ARM_MATH_SIZE_MISMATCH 1

typedef struct {
    uint16_t numRows;
    uint16_t numCols;
    q31_t *pData;
} arm_matrix_instance_q31;

arm_status arm_mat_scale_q31(
    const arm_matrix_instance_q31 *pSrc,
    q31_t scaleFract,
    int32_t shift,
    arm_matrix_instance_q31 *pDst)
{
    q31_t *pIn = pSrc->pData;
    q31_t *pOut = pDst->pData;
    uint32_t numSamples;
    uint32_t blkCnt;
    q31_t in1, in2, out1;
    int32_t totShift;

    if ((pSrc->numRows != pDst->numRows) || (pSrc->numCols != pDst->numCols))
        return ARM_MATH_SIZE_MISMATCH;

    numSamples = (uint32_t)pSrc->numRows * pSrc->numCols;
    totShift = shift + 1;
    blkCnt = numSamples;

    while (blkCnt > 0u)
    {
        in1 = *pIn++;
        in2 = (q31_t)(((q63_t)in1 * (q63_t)scaleFract) >> 32);
        out1 = in2 << totShift;

        if (in2 != (out1 >> totShift))
        {
            if (in2 < 0)
                out1 = -2147483648;
            else
                out1 = 2147483647;
        }

        *pOut++ = out1;
        blkCnt--;
    }

    return ARM_MATH_SUCCESS;
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int32_t *pSrcData, *pDstData;
    int32_t scaleFract;
    int32_t shift;
    size_t rows, cols;
    arm_matrix_instance_q31 src, dst;

    if (nrhs != 3)
        mexErrMsgIdAndTxt("arm_mat_scale_q31:invalidInput", "3 inputs required (matrix, scaleFract, shift).");
    if (nlhs > 1)
        mexErrMsgIdAndTxt("arm_mat_scale_q31:invalidOutput", "At most 1 output allowed.");
    if (!mxIsInt32(prhs[0]) || mxIsComplex(prhs[0]) || mxIsSparse(prhs[0]))
        mexErrMsgIdAndTxt("arm_mat_scale_q31:invalidInputType", "Input 1 must be real int32 matrix.");
    if (!mxIsInt32(prhs[1]) || mxIsComplex(prhs[1]) || mxGetNumberOfElements(prhs[1]) != 1)
        mexErrMsgIdAndTxt("arm_mat_scale_q31:invalidInputType", "Input 2 (scaleFract) must be a real int32 scalar.");
    if (!mxIsInt32(prhs[2]) || mxIsComplex(prhs[2]) || mxGetNumberOfElements(prhs[2]) != 1)
        mexErrMsgIdAndTxt("arm_mat_scale_q31:invalidInputType", "Input 3 (shift) must be a real int32 scalar.");

    rows = mxGetM(prhs[0]);
    cols = mxGetN(prhs[0]);
    pSrcData = (int32_t *)mxGetData(prhs[0]);
    scaleFract = *(int32_t *)mxGetData(prhs[1]);
    shift = *(int32_t *)mxGetData(prhs[2]);

    plhs[0] = mxCreateNumericMatrix(rows, cols, mxINT32_CLASS, mxREAL);
    pDstData = (int32_t *)mxGetData(plhs[0]);

    src.numRows = (uint16_t)rows;
    src.numCols = (uint16_t)cols;
    src.pData = pSrcData;

    dst.numRows = (uint16_t)rows;
    dst.numCols = (uint16_t)cols;
    dst.pData = pDstData;

    arm_mat_scale_q31(&src, scaleFract, shift, &dst);
}