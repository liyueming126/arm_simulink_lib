#include "arm_mat_init_f32_simulink.c"
#include "mex.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    arm_matrix_instance_f32 mat;
    float *pData;
    size_t numRows, numCols;

    if(nrhs != 1) mexErrMsgTxt("One input required (matrix).");
    if(nlhs > 1) mexErrMsgTxt("One output required.");
    if(!mxIsSingle(prhs[0]) || mxIsComplex(prhs[0])) mexErrMsgTxt("Input must be real single.");

    numRows = mxGetM(prhs[0]);
    numCols = mxGetN(prhs[0]);
    pData = (float *)mxGetData(prhs[0]);

    mat.numRows = 0; mat.numCols = 0; mat.pData = NULL;
    arm_mat_init_f32(&mat, (uint16_t)numRows, (uint16_t)numCols, pData);

    plhs[0] = mxCreateNumericMatrix(numRows, numCols, mxSINGLE_CLASS, mxREAL);
    memcpy(mxGetData(plhs[0]), pData, numRows * numCols * sizeof(float));
}