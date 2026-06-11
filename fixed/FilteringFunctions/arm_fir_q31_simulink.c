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

static void arm_fir_q31(
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
    q63_t acc;
    uint32_t numTaps = S->numTaps;
    uint32_t i, tapCnt, blkCnt;

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
            acc += (q63_t)*px++ * *pb++;
            i--;
        } while (i > 0u);

        *pDst++ = (q31_t)(acc >> 31u);

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
