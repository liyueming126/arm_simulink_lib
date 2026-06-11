#include <stdint.h>
#include <string.h>
#include "mex.h"

typedef int32_t q31_t;
typedef int64_t q63_t;

typedef struct
{
    q31_t A0;
    q31_t A1;
    q31_t A2;
    q31_t state[3];
    q31_t Kp;
    q31_t Ki;
    q31_t Kd;
} arm_pid_instance_q31;

q31_t arm_pid_q31(
    arm_pid_instance_q31 * S,
    q31_t in)
{
    q63_t acc;
    q31_t out;

    acc = (q63_t)S->A0 * in;
    acc += (q63_t)S->A1 * S->state[0];
    acc += (q63_t)S->A2 * S->state[1];

    out = (q31_t)(acc >> 31u);
    out += S->state[2];

    S->state[1] = S->state[0];
    S->state[0] = in;
    S->state[2] = out;

    return out;
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    arm_pid_instance_q31 S;
    int32_t *pStruct, *pIn, *pOut;
    size_t blockSize, i;

    if (nrhs != 2)
        mexErrMsgIdAndTxt("arm_pid_q31:invalidInput", "2 inputs required: structVector[9], inSignal.");
    if (nlhs > 1)
        mexErrMsgIdAndTxt("arm_pid_q31:invalidOutput", "At most 1 output allowed.");

    if (!mxIsInt32(prhs[0]) || mxIsComplex(prhs[0]) || mxGetNumberOfElements(prhs[0]) != 9)
        mexErrMsgIdAndTxt("arm_pid_q31:invalidInputType", "Input 1 must be int32 vector of 9 elements.");
    if (!mxIsInt32(prhs[1]) || mxIsComplex(prhs[1]))
        mexErrMsgIdAndTxt("arm_pid_q31:invalidInputType", "Input 2 must be real int32 vector.");

    pStruct = (int32_t *)mxGetData(prhs[0]);
    pIn = (int32_t *)mxGetData(prhs[1]);
    blockSize = mxGetNumberOfElements(prhs[1]);

    plhs[0] = mxCreateNumericArray(mxGetNumberOfDimensions(prhs[1]),
        mxGetDimensions(prhs[1]), mxINT32_CLASS, mxREAL);
    pOut = (int32_t *)mxGetData(plhs[0]);

    S.A0 = pStruct[0];
    S.A1 = pStruct[1];
    S.A2 = pStruct[2];
    S.state[0] = pStruct[3];
    S.state[1] = pStruct[4];
    S.state[2] = pStruct[5];
    S.Kp = pStruct[6];
    S.Ki = pStruct[7];
    S.Kd = pStruct[8];

    for (i = 0; i < blockSize; i++)
    {
        pOut[i] = arm_pid_q31(&S, pIn[i]);
    }
}