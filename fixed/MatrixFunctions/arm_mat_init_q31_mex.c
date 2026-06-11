#include <stdint.h>
#include "mex.h"

typedef int32_t q31_t;

typedef struct {
    uint16_t numRows;
    uint16_t numCols;
    q31_t *pData;
} arm_matrix_instance_q31;

void arm_mat_init_q31(
    arm_matrix_instance_q31 *S,
    uint16_t nRows,
    uint16_t nColumns,
    q31_t *pData)
{
    S->numRows = nRows;
    S->numCols = nColumns;
    S->pData = pData;
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int32_t *pData;
    int32_t *pOut;
    uint16_t nRows, nColumns;
    size_t numel;
    arm_matrix_instance_q31 S;

    if (nrhs != 3)
        mexErrMsgIdAndTxt("arm_mat_init_q31:invalidInput", "3 inputs required (data, nRows, nColumns).");
    if (nlhs > 1)
        mexErrMsgIdAndTxt("arm_mat_init_q31:invalidOutput", "At most 1 output allowed.");
    if (!mxIsInt32(prhs[0]) || mxIsComplex(prhs[0]) || mxIsSparse(prhs[0]))
        mexErrMsgIdAndTxt("arm_mat_init_q31:invalidInputType", "Input 1 must be real int32 array.");
    if (!mxIsInt32(prhs[1]) || mxIsComplex(prhs[1]) || mxGetNumberOfElements(prhs[1]) != 1)
        mexErrMsgIdAndTxt("arm_mat_init_q31:invalidInputType", "Input 2 (nRows) must be a real int32 scalar.");
    if (!mxIsInt32(prhs[2]) || mxIsComplex(prhs[2]) || mxGetNumberOfElements(prhs[2]) != 1)
        mexErrMsgIdAndTxt("arm_mat_init_q31:invalidInputType", "Input 3 (nColumns) must be a real int32 scalar.");

    pData = (int32_t *)mxGetData(prhs[0]);
    nRows = (uint16_t)(*(int32_t *)mxGetData(prhs[1]));
    nColumns = (uint16_t)(*(int32_t *)mxGetData(prhs[2]));
    numel = mxGetNumberOfElements(prhs[0]);

    if ((uint32_t)nRows * (uint32_t)nColumns > numel)
        mexErrMsgIdAndTxt("arm_mat_init_q31:invalidInput", "nRows * nColumns must not exceed number of elements in data.");

    arm_mat_init_q31(&S, nRows, nColumns, pData);

    plhs[0] = mxCreateNumericMatrix(nRows, nColumns, mxINT32_CLASS, mxREAL);
    pOut = (int32_t *)mxGetData(plhs[0]);

    {
        uint32_t i;
        uint32_t n = (uint32_t)nRows * (uint32_t)nColumns;
        for (i = 0u; i < n; i++)
            pOut[i] = pData[i];
    }
}