#include <stdint.h>
#include "mex.h"

typedef int32_t q31_t;
typedef int32_t arm_status;

#define ARM_MATH_SUCCESS       0
#define ARM_MATH_SIZE_MISMATCH 1

typedef struct {
    uint16_t numRows;
    uint16_t numCols;
    q31_t *pData;
} arm_matrix_instance_q31;

arm_status arm_mat_trans_q31(
    const arm_matrix_instance_q31 *pSrc,
    arm_matrix_instance_q31 *pDst)
{
    q31_t *pIn = pSrc->pData;
    q31_t *px;
    uint16_t nRows = pSrc->numRows;
    uint16_t nColumns = pSrc->numCols;
    uint16_t col, row, i;

    if ((pSrc->numRows != pDst->numCols) || (pSrc->numCols != pDst->numRows))
        return ARM_MATH_SIZE_MISMATCH;

    row = nRows;
    i = 0u;

    do
    {
        px = pDst->pData + i;
        col = nColumns;

        while (col > 0u)
        {
            *px = *pIn++;
            px += nRows;
            col--;
        }

        i++;
        row--;
    } while (row > 0u);

    return ARM_MATH_SUCCESS;
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int32_t *pSrcData, *pDstData;
    size_t rows, cols;
    arm_matrix_instance_q31 src, dst;

    if (nrhs != 1)
        mexErrMsgIdAndTxt("arm_mat_trans_q31:invalidInput", "1 input required (matrix).");
    if (nlhs > 1)
        mexErrMsgIdAndTxt("arm_mat_trans_q31:invalidOutput", "At most 1 output allowed.");
    if (!mxIsInt32(prhs[0]) || mxIsComplex(prhs[0]) || mxIsSparse(prhs[0]))
        mexErrMsgIdAndTxt("arm_mat_trans_q31:invalidInputType", "Input must be real int32 matrix.");

    rows = mxGetM(prhs[0]);
    cols = mxGetN(prhs[0]);
    pSrcData = (int32_t *)mxGetData(prhs[0]);

    plhs[0] = mxCreateNumericMatrix(cols, rows, mxINT32_CLASS, mxREAL);
    pDstData = (int32_t *)mxGetData(plhs[0]);

    /* Direct column-major transpose (MATLAB convention) */
    {
        uint16_t r, c;
        for (r = 0; r < rows; r++)
            for (c = 0; c < cols; c++)
                pDstData[c + r * cols] = pSrcData[r + c * rows];
    }
}