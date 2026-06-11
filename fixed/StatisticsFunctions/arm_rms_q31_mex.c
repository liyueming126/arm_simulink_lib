#include <stdint.h>
#include "mex.h"

typedef int32_t q31_t;
typedef int64_t q63_t;

static uint32_t clz_u32(uint32_t x)
{
    if (x == 0) return 32;
    uint32_t n = 0;
    if ((x & 0xFFFF0000u) == 0) { n += 16; x <<= 16; }
    if ((x & 0xFF000000u) == 0) { n += 8;  x <<= 8; }
    if ((x & 0xF0000000u) == 0) { n += 4;  x <<= 4; }
    if ((x & 0xC0000000u) == 0) { n += 2;  x <<= 2; }
    if ((x & 0x80000000u) == 0) { n += 1; }
    return n;
}

static q31_t clip_q63_to_q31(q63_t x)
{
    if (x > 2147483647LL) return 2147483647;
    if (x < -2147483648LL) return (int32_t)-2147483648;
    return (q31_t)x;
}

static void arm_sqrt_q31_portable(q31_t in, q31_t *pOut)
{
    q31_t number, temp1, var1, signBits1, half;
    q31_t bits_val1;

    number = in;

    if (number > 0)
    {
        signBits1 = (q31_t)clz_u32((uint32_t)number) - 1;

        if ((signBits1 & 1u) == 0)
            number = number << signBits1;
        else
            number = number << (signBits1 - 1);

        half = number >> 1;
        temp1 = number;

        {
            float temp_float1 = number * 4.6566128731e-010f;
            union { q31_t i; float f; } conv;
            conv.f = temp_float1;
            bits_val1 = conv.i;
            bits_val1 = 0x5f3759df - (bits_val1 >> 1);
            conv.i = bits_val1;
            temp_float1 = conv.f;
            var1 = (q31_t)(temp_float1 * 1073741824.0f);
        }

        var1 = ((q31_t)((q63_t)var1 * (0x30000000 -
            ((q31_t)((((q31_t)(((q63_t)var1 * var1) >> 31)) *
            (q63_t)half) >> 31))) >> 31)) << 2;

        var1 = ((q31_t)((q63_t)var1 * (0x30000000 -
            ((q31_t)((((q31_t)(((q63_t)var1 * var1) >> 31)) *
            (q63_t)half) >> 31))) >> 31)) << 2;

        var1 = ((q31_t)((q63_t)var1 * (0x30000000 -
            ((q31_t)((((q31_t)(((q63_t)var1 * var1) >> 31)) *
            (q63_t)half) >> 31))) >> 31)) << 2;

        var1 = ((q31_t)(((q63_t)temp1 * var1) >> 31)) << 1;

        if ((signBits1 & 1u) == 0)
            var1 = var1 >> (signBits1 / 2);
        else
            var1 = var1 >> ((signBits1 - 1) / 2);

        *pOut = var1;
    }
    else
    {
        *pOut = 0;
    }
}

void arm_rms_q31(
    q31_t *pSrc,
    uint32_t blockSize,
    q31_t *pResult)
{
    q63_t sum = 0;
    q31_t in;
    uint32_t blkCnt;

    blkCnt = blockSize;

    while (blkCnt > 0u)
    {
        in = *pSrc++;
        sum += (q63_t)in * in;
        blkCnt--;
    }

    arm_sqrt_q31_portable(
        clip_q63_to_q31((sum / (q63_t)blockSize) >> 31),
        pResult);
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int32_t *pSrc;
    size_t blockSize;
    int32_t result;
    const mwSize dims[2] = {1, 1};

    if (nrhs != 1)
        mexErrMsgIdAndTxt("arm_rms_q31:invalidInput", "1 input required.");
    if (nlhs > 1)
        mexErrMsgIdAndTxt("arm_rms_q31:invalidOutput", "At most 1 output allowed.");
    if (!mxIsInt32(prhs[0]) || mxIsComplex(prhs[0]) || mxIsSparse(prhs[0]))
        mexErrMsgIdAndTxt("arm_rms_q31:invalidInputType", "Input must be real int32.");

    blockSize = mxGetNumberOfElements(prhs[0]);
    pSrc = (int32_t *)mxGetData(prhs[0]);

    arm_rms_q31(pSrc, (uint32_t)blockSize, &result);

    plhs[0] = mxCreateNumericArray(2, dims, mxINT32_CLASS, mxREAL);
    ((int32_t *)mxGetData(plhs[0]))[0] = result;
}