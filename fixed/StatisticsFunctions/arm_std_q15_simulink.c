#include <stdint.h>

typedef int16_t q15_t;
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

static void arm_sqrt_q15_portable(q15_t in, q15_t *pOut)
{
    q15_t number, temp1, var1, signBits1, half;
    q31_t bits_val1;

    number = in;

    if (number > 0)
    {
        signBits1 = (q15_t)clz_u32((uint32_t)number) - 17;

        if ((signBits1 & 1u) == 0)
            number = number << signBits1;
        else
            number = number << (signBits1 - 1);

        half = number >> 1;
        temp1 = number;

        {
            float temp_float1 = number * 3.051757812500000e-005f;
            union { q31_t i; float f; } conv;
            conv.f = temp_float1;
            bits_val1 = conv.i;
            bits_val1 = 0x5f3759df - (bits_val1 >> 1);
            conv.i = bits_val1;
            temp_float1 = conv.f;
            var1 = (q31_t)(temp_float1 * 16384.0f);
        }

        var1 = ((q15_t)((q31_t)var1 * (0x3000 -
            ((q15_t)((((q15_t)(((q31_t)var1 * var1) >> 15)) *
            (q31_t)half) >> 15))) >> 15)) << 2;

        var1 = ((q15_t)((q31_t)var1 * (0x3000 -
            ((q15_t)((((q15_t)(((q31_t)var1 * var1) >> 15)) *
            (q31_t)half) >> 15))) >> 15)) << 2;

        var1 = ((q15_t)((q31_t)var1 * (0x3000 -
            ((q15_t)((((q15_t)(((q31_t)var1 * var1) >> 15)) *
            (q31_t)half) >> 15))) >> 15)) << 2;

        var1 = ((q15_t)(((q31_t)temp1 * var1) >> 15)) << 1;

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

void arm_std_q15(
    q15_t *pSrc,
    uint32_t blockSize,
    q15_t *pResult)
{
    q31_t sum = 0;
    q31_t meanOfSquares, squareOfMean;
    uint32_t blkCnt;
    q63_t sumOfSquares = 0;
    q15_t in;
    int32_t satVal;

    if (blockSize == 1)
    {
        *pResult = 0;
        return;
    }

    blkCnt = blockSize;

    while (blkCnt > 0u)
    {
        in = *pSrc++;
        sumOfSquares += ((q63_t)in * in);
        sum += in;
        blkCnt--;
    }

    meanOfSquares = (q31_t)(sumOfSquares / (q63_t)(blockSize - 1u));
    squareOfMean = (q31_t)(((q63_t)sum * sum) / (q63_t)(blockSize * (blockSize - 1u)));

    satVal = ((meanOfSquares - squareOfMean) >> 15);
    if (satVal > 32767) satVal = 32767;
    else if (satVal < -32768) satVal = -32768;

    arm_sqrt_q15_portable((q15_t)satVal, pResult);
}
