#include <stdint.h>
#include <string.h>
#include <limits.h>

typedef int32_t q31_t;
typedef int64_t q63_t;

typedef struct {
    uint16_t numTaps;
    q31_t *pState;
    q31_t *pCoeffs;
} arm_fir_instance_q31;

static void arm_fir_fast_q31(
    const arm_fir_instance_q31 *S,
    q31_t *pSrc,
    q31_t *pDst,
    uint32_t blockSize)
{
    q31_t *pState = S->pState;
    q31_t *pCoeffs = S->pCoeffs;
    q31_t *pStateCurnt;
    q31_t *px;
    q31_t *pb;
    q31_t acc;
    uint32_t numTaps = S->numTaps;
    uint32_t i, blkCnt;

    pStateCurnt = &(S->pState[(numTaps - 1u)]);

    blkCnt = blockSize;

    while (blkCnt > 0u)
    {
        *pStateCurnt++ = *pSrc++;
        acc = 0;
        px = pState;
        pb = pCoeffs;
        i = numTaps;

        do
        {
            acc = (q31_t)((((q63_t)acc << 32) + ((q63_t)*px++ * *pb++)) >> 32);
            i--;
        } while (i > 0u);

        *pDst++ = (q31_t)(acc << 1);

        pState = pState + 1;
        blkCnt--;
    }

    pStateCurnt = S->pState;
    i = (numTaps - 1u);

    while (i > 0u)
    {
        *pStateCurnt++ = *pState++;
        i--;
    }
}
