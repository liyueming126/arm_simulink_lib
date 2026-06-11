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

arm_status arm_mat_mult_fast_q31(
    const arm_matrix_instance_q31 *pSrcA,
    const arm_matrix_instance_q31 *pSrcB,
    arm_matrix_instance_q31 *pDst)
{
    q31_t *pA = pSrcA->pData;
    q31_t *pB = pSrcB->pData;
    q31_t *pC = pDst->pData;
    uint16_t numRowsA = pSrcA->numRows;
    uint16_t numColsA = pSrcA->numCols;
    uint16_t numRowsB = pSrcB->numRows;
    uint16_t numColsB = pSrcB->numCols;
    uint16_t r, c, k;

    if ((pSrcA->numCols != pSrcB->numRows) ||
        (pSrcA->numRows != pDst->numRows) ||
        (pSrcB->numCols != pDst->numCols))
        return ARM_MATH_SIZE_MISMATCH;

    /* Column-major matrix multiply using 64-bit accumulator */
    for (r = 0; r < numRowsA; r++)
    {
        for (c = 0; c < numColsB; c++)
        {
            q63_t sum = 0;
            for (k = 0; k < numColsA; k++)
            {
                sum += (q63_t)pA[r + k * numRowsA] * (q63_t)pB[k + c * numRowsB];
            }
            /* result in 1.31 format: shift right 31 and saturate */
            {
                q31_t res = (q31_t)(sum >> 31);
                pC[r + c * numRowsA] = res;
            }
        }
    }

    return ARM_MATH_SUCCESS;
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int32_t *pSrcDataA, *pSrcDataB, *pDstData;
    size_t rowsA, colsA, rowsB, colsB;
    arm_matrix_instance_q31 srcA, srcB, dst;
    arm_status status;

    if (nrhs != 2)
        mexErrMsgIdAndTxt("arm_mat_mult_fast_q31:invalidInput", "2 inputs required (matrix A, matrix B).");
    if (nlhs > 1)
        mexErrMsgIdAndTxt("arm_mat_mult_fast_q31:invalidOutput", "At most 1 output allowed.");
    if (!mxIsInt32(prhs[0]) || mxIsComplex(prhs[0]) || mxIsSparse(prhs[0]))
        mexErrMsgIdAndTxt("arm_mat_mult_fast_q31:invalidInputType", "Input 1 must be real int32 matrix.");
    if (!mxIsInt32(prhs[1]) || mxIsComplex(prhs[1]) || mxIsSparse(prhs[1]))
        mexErrMsgIdAndTxt("arm_mat_mult_fast_q31:invalidInputType", "Input 2 must be real int32 matrix.");

    rowsA = mxGetM(prhs[0]);
    colsA = mxGetN(prhs[0]);
    rowsB = mxGetM(prhs[1]);
    colsB = mxGetN(prhs[1]);

    if (colsA != rowsB)
        mexErrMsgIdAndTxt("arm_mat_mult_fast_q31:invalidInput", "A_cols must equal B_rows for matrix multiplication.");

    plhs[0] = mxCreateNumericMatrix(rowsA, colsB, mxINT32_CLASS, mxREAL);
    pSrcDataA = (int32_t *)mxGetData(prhs[0]);
    pSrcDataB = (int32_t *)mxGetData(prhs[1]);
    pDstData = (int32_t *)mxGetData(plhs[0]);

    srcA.numRows = (uint16_t)rowsA;
    srcA.numCols = (uint16_t)colsA;
    srcA.pData = pSrcDataA;

    srcB.numRows = (uint16_t)rowsB;
    srcB.numCols = (uint16_t)colsB;
    srcB.pData = pSrcDataB;

    dst.numRows = (uint16_t)rowsA;
    dst.numCols = (uint16_t)colsB;
    dst.pData = pDstData;

    status = arm_mat_mult_fast_q31(&srcA, &srcB, &dst);
    if (status == ARM_MATH_SIZE_MISMATCH)
        mexErrMsgIdAndTxt("arm_mat_mult_fast_q31:sizeMismatch", "Matrix dimension mismatch.");
}