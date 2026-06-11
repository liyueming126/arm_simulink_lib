#include <stdint.h>
#include "mex.h"

typedef int16_t q15_t;
typedef int32_t q31_t;
typedef int32_t arm_status;

#define ARM_MATH_SUCCESS       0
#define ARM_MATH_SIZE_MISMATCH 1

typedef struct {
    uint16_t numRows;
    uint16_t numCols;
    q15_t *pData;
} arm_matrix_instance_q15;

arm_status arm_mat_mult_fast_q15(
    const arm_matrix_instance_q15 *pSrcA,
    const arm_matrix_instance_q15 *pSrcB,
    arm_matrix_instance_q15 *pDst,
    q15_t *pState)
{
    q15_t *pA = pSrcA->pData;
    q15_t *pB = pSrcB->pData;
    q15_t *pC = pDst->pData;
    uint16_t numRowsA = pSrcA->numRows;
    uint16_t numColsA = pSrcA->numCols;
    uint16_t numRowsB = pSrcB->numRows;
    uint16_t numColsB = pSrcB->numCols;
    uint16_t r, c, k;
    q31_t sum;
    (void)pState;

    if ((pSrcA->numCols != pSrcB->numRows) ||
        (pSrcA->numRows != pDst->numRows) ||
        (pSrcB->numCols != pDst->numCols))
        return ARM_MATH_SIZE_MISMATCH;

    /* Column-major matrix multiply with 32-bit accumulator */
    for (r = 0; r < numRowsA; r++)
    {
        for (c = 0; c < numColsB; c++)
        {
            sum = 0;
            for (k = 0; k < numColsA; k++)
            {
                sum += (q31_t)pA[r + k * numRowsA] * (q31_t)pB[k + c * numRowsB];
            }
            sum = sum >> 15;
            if (sum > 32767) sum = 32767;
            else if (sum < -32768) sum = -32768;
            pC[r + c * numRowsA] = (q15_t)sum;
        }
    }

    return ARM_MATH_SUCCESS;
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int16_t *pSrcDataA, *pSrcDataB, *pDstData;
    size_t rowsA, colsA, rowsB, colsB;
    arm_matrix_instance_q15 srcA, srcB, dst;
    arm_status status;

    if (nrhs != 2)
        mexErrMsgIdAndTxt("arm_mat_mult_fast_q15:invalidInput", "2 inputs required (matrix A, matrix B).");
    if (nlhs > 1)
        mexErrMsgIdAndTxt("arm_mat_mult_fast_q15:invalidOutput", "At most 1 output allowed.");
    if (!mxIsInt16(prhs[0]) || mxIsComplex(prhs[0]) || mxIsSparse(prhs[0]))
        mexErrMsgIdAndTxt("arm_mat_mult_fast_q15:invalidInputType", "Input 1 must be real int16 matrix.");
    if (!mxIsInt16(prhs[1]) || mxIsComplex(prhs[1]) || mxIsSparse(prhs[1]))
        mexErrMsgIdAndTxt("arm_mat_mult_fast_q15:invalidInputType", "Input 2 must be real int16 matrix.");

    rowsA = mxGetM(prhs[0]);
    colsA = mxGetN(prhs[0]);
    rowsB = mxGetM(prhs[1]);
    colsB = mxGetN(prhs[1]);

    if (colsA != rowsB)
        mexErrMsgIdAndTxt("arm_mat_mult_fast_q15:invalidInput", "A_cols must equal B_rows for matrix multiplication.");

    plhs[0] = mxCreateNumericMatrix(rowsA, colsB, mxINT16_CLASS, mxREAL);
    pSrcDataA = (int16_t *)mxGetData(prhs[0]);
    pSrcDataB = (int16_t *)mxGetData(prhs[1]);
    pDstData = (int16_t *)mxGetData(plhs[0]);

    srcA.numRows = (uint16_t)rowsA;
    srcA.numCols = (uint16_t)colsA;
    srcA.pData = pSrcDataA;

    srcB.numRows = (uint16_t)rowsB;
    srcB.numCols = (uint16_t)colsB;
    srcB.pData = pSrcDataB;

    dst.numRows = (uint16_t)rowsA;
    dst.numCols = (uint16_t)colsB;
    dst.pData = pDstData;

    status = arm_mat_mult_fast_q15(&srcA, &srcB, &dst, (q15_t *)0);
    if (status == ARM_MATH_SIZE_MISMATCH)
        mexErrMsgIdAndTxt("arm_mat_mult_fast_q15:sizeMismatch", "Matrix dimension mismatch.");
}