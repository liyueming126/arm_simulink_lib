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

static q31_t clip_q63_to_q31(q63_t x)
{
    if (x > 2147483647LL)
        return 2147483647;
    if (x < (-2147483647LL - 1))
        return (-2147483647 - 1);
    return (q31_t)x;
}

arm_status arm_mat_cmplx_mult_q31(
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
    q63_t sumReal, sumImag;
    q31_t a_re, a_im, b_re, b_im;

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

                sumReal += (q63_t)a_re * (q63_t)b_re;
                sumReal -= (q63_t)a_im * (q63_t)b_im;
                sumImag += (q63_t)a_re * (q63_t)b_im;
                sumImag += (q63_t)a_im * (q63_t)b_re;
            }

            pC[r + 2*c * numRowsA] = clip_q63_to_q31(sumReal >> 31);
            pC[r + (2*c+1) * numRowsA] = clip_q63_to_q31(sumImag >> 31);
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
        mexErrMsgIdAndTxt("arm_mat_cmplx_mult_q31:invalidInput", "2 inputs required (matrix A, matrix B).");
    if (nlhs > 1)
        mexErrMsgIdAndTxt("arm_mat_cmplx_mult_q31:invalidOutput", "At most 1 output allowed.");
    if (!mxIsInt32(prhs[0]) || mxIsComplex(prhs[0]) || mxIsSparse(prhs[0]))
        mexErrMsgIdAndTxt("arm_mat_cmplx_mult_q31:invalidInputType", "Input 1 must be real int32 matrix with [real imag] pairs.");
    if (!mxIsInt32(prhs[1]) || mxIsComplex(prhs[1]) || mxIsSparse(prhs[1]))
        mexErrMsgIdAndTxt("arm_mat_cmplx_mult_q31:invalidInputType", "Input 2 must be real int32 matrix with [real imag] pairs.");

    rowsA = mxGetM(prhs[0]);
    colsA = mxGetN(prhs[0]);
    rowsB = mxGetM(prhs[1]);
    colsB = mxGetN(prhs[1]);

    if ((colsA / 2) != (int)rowsB)
        mexErrMsgIdAndTxt("arm_mat_cmplx_mult_q31:invalidInput", "A_cols/2 must equal B_rows for complex matrix multiplication.");

    plhs[0] = mxCreateNumericMatrix(rowsA, colsB, mxINT32_CLASS, mxREAL);
    pSrcDataA = (int32_t *)mxGetData(prhs[0]);
    pSrcDataB = (int32_t *)mxGetData(prhs[1]);
    pDstData = (int32_t *)mxGetData(plhs[0]);

    srcA.numRows = (uint16_t)rowsA;
    srcA.numCols = (uint16_t)(colsA / 2);
    srcA.pData = pSrcDataA;

    srcB.numRows = (uint16_t)rowsB;
    srcB.numCols = (uint16_t)(colsB / 2);
    srcB.pData = pSrcDataB;

    dst.numRows = (uint16_t)rowsA;
    dst.numCols = (uint16_t)(colsB / 2);
    dst.pData = pDstData;

    status = arm_mat_cmplx_mult_q31(&srcA, &srcB, &dst);
    if (status == ARM_MATH_SIZE_MISMATCH)
        mexErrMsgIdAndTxt("arm_mat_cmplx_mult_q31:sizeMismatch", "Matrix dimension mismatch.");
}