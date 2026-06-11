#include <stdint.h>
#include <string.h>

typedef int8_t q7_t;

typedef struct {
    uint16_t numTaps;
    q7_t *pState;
    q7_t *pCoeffs;
} arm_fir_instance_q7;

static void arm_fir_q7(
    const arm_fir_instance_q7 *S,
    q7_t *pSrc,
    q7_t *pDst,
    uint32_t blockSize)
{
    q7_t *pState = S->pState;
    q7_t *pCoeffs = S->pCoeffs;
    q7_t *pStateCurnt;
    q7_t *px;
    q7_t *pb;
    int32_t acc;
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
        tapCnt = numTaps;

        do
        {
            acc += (int16_t)*px++ * (int16_t)*pb++;
            tapCnt--;
        } while (tapCnt > 0u);

        acc = acc >> 7;
        if (acc > 127) acc = 127;
        if (acc < -128) acc = -128;
        *pDst++ = (q7_t)acc;

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
