#include <stdint.h>
#include "mex.h"

typedef int16_t q15_t;
typedef int32_t q31_t;
typedef int64_t q63_t;
typedef int32_t arm_status;

#define ARM_MATH_SUCCESS       0
#define ARM_MATH_SIZE_MISMATCH 1

typedef struct {
    uint16_t numRows;
    uint16_t numCols;
    q15_t *pData;
} arm_matrix_instance_q15;

arm_status arm_mat_cmplx_mult_q15(
    const arm_matrix_instance_q15 *pSrcA,
    const arm_matrix_instance_q15 *pSrcB,
    arm_matrix_instance_q15 *pDst,
    q15_t *pScratch)
{
    q15_t *pA = pSrcA->pData;
    q15_t *pB = pSrcB->pData;
    q15_t *pC = pDst->pData;
    uint16_t numRowsA = pSrcA->numRows;
    uint16_t numColsA = pSrcA->numCols;
    uint16_t numRowsB = pSrcB->numRows;
    uint16_t numColsB = pSrcB->numCols;
    uint16_t r, c, k;
    q63_t sumReal, sumImag;
    q15_t a_re, a_im, b_re, b_im;
    (void)pScratch;

    if ((pSrcA->numCols != pSrcB->numRows) ||
        (pSrcA->numRows != pDst->numRows) ||
        (pSrcB->numCols != pDst->numCols))
        return ARM_MATH_SIZE_MISMATCH;

    /* Column-major complex matrix multiply.
       Data stored interleaved: each complex column uses 2 real columns.
       A[r][k] complex: real at pA[r + 2*k*numRowsA], imag at pA[r + (2*k+1)*numRowsA]
       B[k][c] complex: real at pB[k + 2*c*numRowsB], imag at pB[k + (2*c+1)*numRowsB]
       C[r][c] complex: real at pC[r + 2*c*numRowsA], imag at pC[r + (2*c+1)*numRowsA] */
    for (r = 0; r < numRowsA; r++)
    {
        for (c = 0; c < numColsB; c++)
        {
            sumReal = 0;
            sumImag = 0;
            for (k = 0; k < numColsA; k++)
            {
                a_re = pA[r + 2*k * numRowsA];
                a_im = pA[r + (2*k+1) * numRowsA];
                b_re = pB[k + 2*c * numRowsB];
                b_im = pB[k + (2*c+1) * numRowsB];

                sumReal += (q31_t)a_re * (q31_t)b_re;
                sumReal -= (q31_t)a_im * (q31_t)b_im;
                sumImag += (q31_t)a_re * (q31_t)b_im;
                sumImag += (q31_t)a_im * (q31_t)b_re;
            }

            sumReal = sumReal >> 15;
            sumImag = sumImag >> 15;

            if (sumReal > 32767) sumReal = 32767;
            else if (sumReal < -32768) sumReal = -32768;
            if (sumImag > 32767) sumImag = 32767;
            else if (sumImag < -32768) sumImag = -32768;

            pC[r + 2*c * numRowsA] = (q15_t)sumReal;
            pC[r + (2*c+1) * numRowsA] = (q15_t)sumImag;
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
        mexErrMsgIdAndTxt("arm_mat_cmplx_mult_q15:invalidInput", "2 inputs required (matrix A, matrix B).");
    if (nlhs > 1)
        mexErrMsgIdAndTxt("arm_mat_cmplx_mult_q15:invalidOutput", "At most 1 output allowed.");
    if (!mxIsInt16(prhs[0]) || mxIsComplex(prhs[0]) || mxIsSparse(prhs[0]))
        mexErrMsgIdAndTxt("arm_mat_cmplx_mult_q15:invalidInputType", "Input 1 must be real int16 matrix with [real imag] pairs along columns.");
    if (!mxIsInt16(prhs[1]) || mxIsComplex(prhs[1]) || mxIsSparse(prhs[1]))
        mexErrMsgIdAndTxt("arm_mat_cmplx_mult_q15:invalidInputType", "Input 2 must be real int16 matrix with [real imag] pairs along columns.");

    rowsA = mxGetM(prhs[0]);
    colsA = mxGetN(prhs[0]);
    rowsB = mxGetM(prhs[1]);
    colsB = mxGetN(prhs[1]);

    if ((colsA / 2) != (int)rowsB)
        mexErrMsgIdAndTxt("arm_mat_cmplx_mult_q15:invalidInput", "A_cols/2 must equal B_rows for complex matrix multiplication.");

    plhs[0] = mxCreateNumericMatrix(rowsA, colsB, mxINT16_CLASS, mxREAL);
    pSrcDataA = (int16_t *)mxGetData(prhs[0]);
    pSrcDataB = (int16_t *)mxGetData(prhs[1]);
    pDstData = (int16_t *)mxGetData(plhs[0]);

    srcA.numRows = (uint16_t)rowsA;
    srcA.numCols = (uint16_t)(colsA / 2);
    srcA.pData = pSrcDataA;

    srcB.numRows = (uint16_t)rowsB;
    srcB.numCols = (uint16_t)(colsB / 2);
    srcB.pData = pSrcDataB;

    dst.numRows = (uint16_t)rowsA;
    dst.numCols = (uint16_t)(colsB / 2);
    dst.pData = pDstData;

    status = arm_mat_cmplx_mult_q15(&srcA, &srcB, &dst, (q15_t *)0);
    if (status == ARM_MATH_SIZE_MISMATCH)
        mexErrMsgIdAndTxt("arm_mat_cmplx_mult_q15:sizeMismatch", "Matrix dimension mismatch.");
}