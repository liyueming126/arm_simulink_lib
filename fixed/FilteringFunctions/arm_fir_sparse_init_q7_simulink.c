#include <stdint.h>
#include <string.h>

typedef int8_t q7_t;

typedef struct {
    uint16_t numTaps;
    uint16_t stateIndex;
    q7_t *pState;
    q7_t *pCoeffs;
    uint16_t maxDelay;
    int32_t *pTapDelay;
} arm_fir_sparse_instance_q7;
