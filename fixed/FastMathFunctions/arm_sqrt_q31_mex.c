#include <stdint.h>

#define ARM_MATH_SUCCESS         0
#define ARM_MATH_ARGUMENT_ERROR -1

static int32_t arm_clz_u32(uint32_t x)
{
    int32_t cnt = 0;
    if (x == 0) return 32;
    if ((x & 0xFFFF0000u) == 0) { cnt += 16; x <<= 16; }
    if ((x & 0xFF000000u) == 0) { cnt += 8;  x <<= 8;  }
    if ((x & 0xF0000000u) == 0) { cnt += 4;  x <<= 4;  }
    if ((x & 0xC0000000u) == 0) { cnt += 2;  x <<= 2;  }
    if ((x & 0x80000000u) == 0) { cnt += 1;  }
    return cnt;
}

typedef union {
    int32_t fracval;
    float   floatval;
} float_conv_t;

static int32_t arm_sqrt_q31_scalar(int32_t in, int32_t *pOut)
{
    int32_t number, temp1, bits_val1, var1, signBits1, half;
    float temp_float1;
    float_conv_t tempconv;

    number = in;

    if (number > 0) {
        signBits1 = arm_clz_u32((uint32_t)number) - 1;

        if ((signBits1 % 2) == 0) {
            number = number << signBits1;
        } else {
            number = number << (signBits1 - 1);
        }

        half = number >> 1;
        temp1 = number;

        temp_float1 = (float)number * 4.6566128731e-010f;
        tempconv.floatval = temp_float1;
        bits_val1 = tempconv.fracval;
        bits_val1 = 0x5f3759df - (bits_val1 >> 1);
        tempconv.fracval = bits_val1;
        temp_float1 = tempconv.floatval;
        var1 = (int32_t)(temp_float1 * 1073741824.0f);

        /* Newton-Raphson iterations */
        {
            int32_t nr_iter, vv_hi, factor;
            int64_t vv, corr;
            for (nr_iter = 0; nr_iter < 4; nr_iter++)
            {
                vv = (int64_t)var1 * var1;
                vv_hi = (int32_t)(vv >> 31);
                corr = (int64_t)vv_hi * half;
                factor = (int32_t)(0x30000000LL - (int32_t)(corr >> 31));
                var1 = (int32_t)(((int64_t)var1 * factor) >> 31) << 2;
            }
        }

        var1 = ((int32_t)(((int64_t)temp1 * var1) >> 31)) << 1;

        if ((signBits1 % 2) == 0) {
            var1 = var1 >> (signBits1 / 2);
        } else {
            var1 = var1 >> ((signBits1 - 1) / 2);
        }
        *pOut = var1;
        return ARM_MATH_SUCCESS;
    } else {
        *pOut = 0;
        return ARM_MATH_ARGUMENT_ERROR;
    }
}

#include "mex.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int32_t *in, *out;
    mwSize i, N;

    if (nrhs != 1) {
        mexErrMsgIdAndTxt("arm_sqrt_q31:input", "One input required.");
    }
    if (!mxIsInt32(prhs[0])) {
        mexErrMsgIdAndTxt("arm_sqrt_q31:input", "Input must be int32.");
    }

    N = mxGetNumberOfElements(prhs[0]);
    in = (int32_t *)mxGetData(prhs[0]);

    plhs[0] = mxCreateNumericMatrix(1, (mwSize)N, mxINT32_CLASS, mxREAL);
    out = (int32_t *)mxGetData(plhs[0]);

    for (i = 0; i < N; i++) {
        int32_t val;
        arm_sqrt_q31_scalar(in[i], &val);
        out[i] = val;
    }
}