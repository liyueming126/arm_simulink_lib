#include <stdint.h>

typedef int16_t q15_t;
typedef int32_t q31_t;
typedef float float32_t;

void arm_bitreversal_f32(
    float32_t *pSrc,
    uint16_t fftSize,
    uint16_t bitRevFactor,
    uint16_t *pBitRevTab)
{
    uint16_t fftLenBy2, fftLenBy2p1;
    uint16_t i, j;
    float32_t in;

    j = 0u;
    fftLenBy2 = fftSize >> 1u;
    fftLenBy2p1 = (fftSize >> 1u) + 1u;

    for (i = 0u; i <= (fftLenBy2 - 2u); i += 2u)
    {
        if (i < j)
        {
            in = pSrc[2u * i];
            pSrc[2u * i] = pSrc[2u * j];
            pSrc[2u * j] = in;

            in = pSrc[(2u * i) + 1u];
            pSrc[(2u * i) + 1u] = pSrc[(2u * j) + 1u];
            pSrc[(2u * j) + 1u] = in;

            in = pSrc[2u * (i + fftLenBy2p1)];
            pSrc[2u * (i + fftLenBy2p1)] = pSrc[2u * (j + fftLenBy2p1)];
            pSrc[2u * (j + fftLenBy2p1)] = in;

            in = pSrc[(2u * (i + fftLenBy2p1)) + 1u];
            pSrc[(2u * (i + fftLenBy2p1)) + 1u] = pSrc[(2u * (j + fftLenBy2p1)) + 1u];
            pSrc[(2u * (j + fftLenBy2p1)) + 1u] = in;
        }

        in = pSrc[2u * (i + 1u)];
        pSrc[2u * (i + 1u)] = pSrc[2u * (j + fftLenBy2)];
        pSrc[2u * (j + fftLenBy2)] = in;

        in = pSrc[(2u * (i + 1u)) + 1u];
        pSrc[(2u * (i + 1u)) + 1u] = pSrc[(2u * (j + fftLenBy2)) + 1u];
        pSrc[(2u * (j + fftLenBy2)) + 1u] = in;

        j = *pBitRevTab;
        pBitRevTab += bitRevFactor;
    }
}

void arm_bitreversal_q31(
    q31_t *pSrc,
    uint32_t fftLen,
    uint16_t bitRevFactor,
    uint16_t *pBitRevTable)
{
    uint32_t fftLenBy2, fftLenBy2p1, i, j;
    q31_t in;

    j = 0u;
    fftLenBy2 = fftLen / 2u;
    fftLenBy2p1 = (fftLen / 2u) + 1u;

    for (i = 0u; i <= (fftLenBy2 - 2u); i += 2u)
    {
        if (i < j)
        {
            in = pSrc[2u * i];
            pSrc[2u * i] = pSrc[2u * j];
            pSrc[2u * j] = in;

            in = pSrc[(2u * i) + 1u];
            pSrc[(2u * i) + 1u] = pSrc[(2u * j) + 1u];
            pSrc[(2u * j) + 1u] = in;

            in = pSrc[2u * (i + fftLenBy2p1)];
            pSrc[2u * (i + fftLenBy2p1)] = pSrc[2u * (j + fftLenBy2p1)];
            pSrc[2u * (j + fftLenBy2p1)] = in;

            in = pSrc[(2u * (i + fftLenBy2p1)) + 1u];
            pSrc[(2u * (i + fftLenBy2p1)) + 1u] = pSrc[(2u * (j + fftLenBy2p1)) + 1u];
            pSrc[(2u * (j + fftLenBy2p1)) + 1u] = in;
        }

        in = pSrc[2u * (i + 1u)];
        pSrc[2u * (i + 1u)] = pSrc[2u * (j + fftLenBy2)];
        pSrc[2u * (j + fftLenBy2)] = in;

        in = pSrc[(2u * (i + 1u)) + 1u];
        pSrc[(2u * (i + 1u)) + 1u] = pSrc[(2u * (j + fftLenBy2)) + 1u];
        pSrc[(2u * (j + fftLenBy2)) + 1u] = in;

        j = *pBitRevTable;
        pBitRevTable += bitRevFactor;
    }
}

void arm_bitreversal_q15(
    q15_t *pSrc16,
    uint32_t fftLen,
    uint16_t bitRevFactor,
    uint16_t *pBitRevTab)
{
    q31_t *pSrc = (q31_t *)pSrc16;
    q31_t in;
    uint32_t fftLenBy2, fftLenBy2p1;
    uint32_t i, j;

    j = 0u;
    fftLenBy2 = fftLen / 2u;
    fftLenBy2p1 = (fftLen / 2u) + 1u;

    for (i = 0u; i <= (fftLenBy2 - 2u); i += 2u)
    {
        if (i < j)
        {
            in = pSrc[i];
            pSrc[i] = pSrc[j];
            pSrc[j] = in;

            in = pSrc[i + fftLenBy2p1];
            pSrc[i + fftLenBy2p1] = pSrc[j + fftLenBy2p1];
            pSrc[j + fftLenBy2p1] = in;
        }

        in = pSrc[i + 1u];
        pSrc[i + 1u] = pSrc[j + fftLenBy2];
        pSrc[j + fftLenBy2] = in;

        j = *pBitRevTab;
        pBitRevTab += bitRevFactor;
    }
}