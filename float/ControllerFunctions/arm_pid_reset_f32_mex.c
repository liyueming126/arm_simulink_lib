#include <stdint.h>
#include <string.h>
#include "mex.h"

typedef struct
{
    float A0;
    float A1;
    float A2;
    float state[3];
    float Kp;
    float Ki;
    float Kd;
} arm_pid_instance_f32;

void arm_pid_reset_f32(
    arm_pid_instance_f32 * S)
{
    memset(S->state, 0, 3u * sizeof(float));
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    arm_pid_instance_f32 S;
    float *pIn, *pOut;

    if (nrhs != 1)
        mexErrMsgIdAndTxt("arm_pid_reset_f32:invalidInput", "1 input required: struct vector [A0,A1,A2,state0,state1,state2,Kp,Ki,Kd].");
    if (nlhs > 1)
        mexErrMsgIdAndTxt("arm_pid_reset_f32:invalidOutput", "At most 1 output allowed.");

    if (!mxIsSingle(prhs[0]) || mxIsComplex(prhs[0]) || mxGetNumberOfElements(prhs[0]) != 9)
        mexErrMsgIdAndTxt("arm_pid_reset_f32:invalidInputType", "Input must be single vector of 9 elements.");

    pIn = (float *)mxGetData(prhs[0]);
    S.A0 = pIn[0];
    S.A1 = pIn[1];
    S.A2 = pIn[2];
    S.state[0] = pIn[3];
    S.state[1] = pIn[4];
    S.state[2] = pIn[5];
    S.Kp = pIn[6];
    S.Ki = pIn[7];
    S.Kd = pIn[8];

    arm_pid_reset_f32(&S);

    plhs[0] = mxCreateNumericMatrix(1, 9, mxSINGLE_CLASS, mxREAL);
    pOut = (float *)mxGetData(plhs[0]);
    pOut[0] = S.A0;
    pOut[1] = S.A1;
    pOut[2] = S.A2;
    pOut[3] = S.state[0];
    pOut[4] = S.state[1];
    pOut[5] = S.state[2];
    pOut[6] = S.Kp;
    pOut[7] = S.Ki;
    pOut[8] = S.Kd;
}