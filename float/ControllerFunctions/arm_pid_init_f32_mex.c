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

void arm_pid_init_f32(
    arm_pid_instance_f32 * S,
    int32_t resetStateFlag)
{
    /* Derived coefficient A0 */
    S->A0 = S->Kp + S->Ki + S->Kd;

    /* Derived coefficient A1 */
    S->A1 = (-S->Kp) - ((float)2.0 * S->Kd);

    /* Derived coefficient A2 */
    S->A2 = S->Kd;

    /* Check whether state needs reset or not */
    if (resetStateFlag)
    {
        memset(S->state, 0, 3u * sizeof(float));
    }
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    arm_pid_instance_f32 S;
    float *pOut;

    if (nrhs != 4)
        mexErrMsgIdAndTxt("arm_pid_init_f32:invalidInput", "4 inputs required: Kp, Ki, Kd, resetStateFlag.");
    if (nlhs > 1)
        mexErrMsgIdAndTxt("arm_pid_init_f32:invalidOutput", "At most 1 output allowed.");

    if (!mxIsSingle(prhs[0]) || mxIsComplex(prhs[0]) || mxGetNumberOfElements(prhs[0]) != 1)
        mexErrMsgIdAndTxt("arm_pid_init_f32:invalidInputType", "Kp must be scalar single.");
    if (!mxIsSingle(prhs[1]) || mxIsComplex(prhs[1]) || mxGetNumberOfElements(prhs[1]) != 1)
        mexErrMsgIdAndTxt("arm_pid_init_f32:invalidInputType", "Ki must be scalar single.");
    if (!mxIsSingle(prhs[2]) || mxIsComplex(prhs[2]) || mxGetNumberOfElements(prhs[2]) != 1)
        mexErrMsgIdAndTxt("arm_pid_init_f32:invalidInputType", "Kd must be scalar single.");
    if (!mxIsDouble(prhs[3]) || mxIsComplex(prhs[3]) || mxGetNumberOfElements(prhs[3]) != 1)
        mexErrMsgIdAndTxt("arm_pid_init_f32:invalidInputType", "resetStateFlag must be scalar double.");

    S.Kp = *(float *)mxGetData(prhs[0]);
    S.Ki = *(float *)mxGetData(prhs[1]);
    S.Kd = *(float *)mxGetData(prhs[2]);
    int32_t resetFlag = (int32_t)mxGetScalar(prhs[3]);

    arm_pid_init_f32(&S, resetFlag);

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