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

arm_status arm_mat_sub_q15(
    const arm_matrix_instance_q15 *pSrcA,
    const arm_matrix_instance_q15 *pSrcB,
    arm_matrix_instance_q15 *pDst)
{
    q15_t *pInA = pSrcA->pData;
    q15_t *pInB = pSrcB->pData;
    q15_t *pOut = pDst->pData;
    uint32_t numSamples;
    uint32_t blkCnt;
    int32_t diff;

    if ((pSrcA->numRows != pSrcB->numRows) ||
        (pSrcA->numCols != pSrcB->numCols) ||
        (pSrcA->numRows != pDst->numRows) ||
        (pSrcA->numCols != pDst->numCols))
    {
        return ARM_MATH_SIZE_MISMATCH;
    }

    numSamples = (uint32_t)pSrcA->numRows * pSrcA->numCols;
    blkCnt = numSamples;

    while (blkCnt > 0u)
    {
        diff = (int32_t)*pInA++ - (int32_t)*pInB++;

        if (diff > 32767)
            diff = 32767;
        else if (diff < -32768)
            diff = -32768;

        *pOut++ = (q15_t)diff;
        blkCnt--;
    }

    return ARM_MATH_SUCCESS;
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int16_t *pSrcDataA, *pSrcDataB, *pDstData;
    size_t rowsA, colsA, rowsB, colsB;
    arm_matrix_instance_q15 srcA, srcB, dst;

    if (nrhs != 2)
        mexErrMsgIdAndTxt("arm_mat_sub_q15:invalidInput", "2 inputs required (matrix A, matrix B).");
    if (nlhs > 1)
        mexErrMsgIdAndTxt("arm_mat_sub_q15:invalidOutput", "At most 1 output allowed.");
    if (!mxIsInt16(prhs[0]) || mxIsComplex(prhs[0]) || mxIsSparse(prhs[0]))
        mexErrMsgIdAndTxt("arm_mat_sub_q15:invalidInputType", "Input 1 must be real int16.");
    if (!mxIsInt16(prhs[1]) || mxIsComplex(prhs[1]) || mxIsSparse(prhs[1]))
        mexErrMsgIdAndTxt("arm_mat_sub_q15:invalidInputType", "Input 2 must be real int16.");

    rowsA = mxGetM(prhs[0]);
    colsA = mxGetN(prhs[0]);
    rowsB = mxGetM(prhs[1]);
    colsB = mxGetN(prhs[1]);

    if (rowsA != rowsB || colsA != colsB)
        mexErrMsgIdAndTxt("arm_mat_sub_q15:invalidInput", "Input matrices must have the same dimensions.");

    plhs[0] = mxCreateNumericMatrix(rowsA, colsA, mxINT16_CLASS, mxREAL);
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
    dst.numCols = (uint16_t)colsA;
    dst.pData = pDstData;

    arm_mat_sub_q15(&srcA, &srcB, &dst);
}