#include "arm_mat_scale_f32_simulink.c"
#include "mex.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    arm_matrix_instance_f32 matSrc, matDst;
    float *pSrc, *pD;
    float scale;
    size_t numRows, numCols;
    arm_status status;

    if(nrhs != 2) mexErrMsgTxt("Two inputs required (matrix, scalar).");
    if(nlhs > 1) mexErrMsgTxt("One output required.");
    if(!mxIsSingle(prhs[0]) || mxIsComplex(prhs[0])) mexErrMsgTxt("Input matrix must be real single.");
    if(!mxIsSingle(prhs[1]) || mxGetNumberOfElements(prhs[1]) != 1)
        mexErrMsgTxt("Scale factor must be a real single scalar.");

    numRows = mxGetM(prhs[0]);
    numCols = mxGetN(prhs[0]);
    pSrc = (float *)mxGetData(prhs[0]);
    scale = *(float *)mxGetData(prhs[1]);

    matSrc.numRows = (uint16_t)numRows; matSrc.numCols = (uint16_t)numCols; matSrc.pData = pSrc;

    plhs[0] = mxCreateNumericMatrix(numRows, numCols, mxSINGLE_CLASS, mxREAL);
    pD = (float *)mxGetData(plhs[0]);
    matDst.numRows = (uint16_t)numRows; matDst.numCols = (uint16_t)numCols; matDst.pData = pD;

    status = arm_mat_scale_f32(&matSrc, scale, &matDst);
    if(status != ARM_MATH_SUCCESS) mexErrMsgTxt("Matrix scaling failed.");
}