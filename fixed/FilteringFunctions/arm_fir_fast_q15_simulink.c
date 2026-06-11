#include <stdint.h>
#include <string.h>
#include <limits.h>

typedef int16_t q15_t;
typedef int32_t q31_t;

typedef struct {
    uint16_t numTaps;
    q15_t *pState;
    q15_t *pCoeffs;
} arm_fir_instance_q15;

static void arm_fir_fast_q15(
    const arm_fir_instance_q15 *S,
    q15_t *pSrc,
    q15_t *pDst,
    uint32_t blockSize)
{
    q15_t *pState = S->pState;
    q15_t *pCoeffs = S->pCoeffs;
    q15_t *pStateCurnt;
    q15_t *px;
    q15_t *pb;
    q31_t acc;
    uint32_t numTaps = S->numTaps;
    uint32_t tapCnt, blkCnt;

    pStateCurnt = &(S->pState[(numTaps - 1u)]);

    blkCnt = blockSize;

    while (blkCnt > 0u)
    {
        *pStateCurnt++ = *pSrc++;
        acc = 0;
        px = pState;
        pb = pCoeffs;
        tapCnt = numTaps >> 1u;

        do
        {
            acc += (q31_t)*px++ * *pb++;
            acc += (q31_t)*px++ * *pb++;
            tapCnt--;
        } while (tapCnt > 0u);

        if (acc > 0)
        {
            acc = (acc >> 15);
            *pDst++ = (q15_t)(acc > 32767 ? 32767 : acc);
        }
        else
        {
            acc = (acc >> 15);
            *pDst++ = (q15_t)(acc < -32768 ? -32768 : acc);
        }

        pState = pState + 1;
        blkCnt--;
    }

    pStateCurnt = S->pState;
    tapCnt = (numTaps - 1u);

    while (tapCnt > 0u)
    {
        *pStateCurnt++ = *pState++;
        tapCnt--;
    }
}
