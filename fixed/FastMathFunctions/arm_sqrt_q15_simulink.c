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

static int32_t arm_sqrt_q15_scalar(int16_t in, int16_t *pOut)
{
    int16_t number, temp1, var1, signBits1, half;
    int32_t bits_val1;
    float temp_float1;
    float_conv_t tempconv;

    number = in;

    if (number > 0) {
        signBits1 = arm_clz_u32((uint32_t)(uint16_t)number) - 17;

        if ((signBits1 % 2) == 0) {
            number = number << signBits1;
        } else {
            number = number << (signBits1 - 1);
        }

        half = number >> 1;
        temp1 = number;

        temp_float1 = (float)number * 3.051757812500000e-005f;
        tempconv.floatval = temp_float1;
        bits_val1 = tempconv.fracval;
        bits_val1 = 0x5f3759df - (bits_val1 >> 1);
        tempconv.fracval = bits_val1;
        temp_float1 = tempconv.floatval;
        var1 = (int16_t)(temp_float1 * 16384.0f);

        var1 = (int16_t)(((int32_t)var1 * (0x3000 -
            (int16_t)((((int16_t)(((int32_t)var1 * var1) >> 15)) *
            (int32_t)half) >> 15))) >> 15) << 2;

        var1 = (int16_t)(((int32_t)var1 * (0x3000 -
            (int16_t)((((int16_t)(((int32_t)var1 * var1) >> 15)) *
            (int32_t)half) >> 15))) >> 15) << 2;

        var1 = (int16_t)(((int32_t)var1 * (0x3000 -
            (int16_t)((((int16_t)(((int32_t)var1 * var1) >> 15)) *
            (int32_t)half) >> 15))) >> 15) << 2;

        var1 = (int16_t)((((int32_t)temp1 * var1) >> 15)) << 1;

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
