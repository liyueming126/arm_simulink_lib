#include <stdint.h>

typedef int32_t q31_t;

typedef struct {
    uint16_t numRows;
    uint16_t numCols;
    q31_t *pData;
} arm_matrix_instance_q31;

void arm_mat_init_q31(
    arm_matrix_instance_q31 *S,
    uint16_t nRows,
    uint16_t nColumns,
    q31_t *pData)
{
    S->numRows = nRows;
    S->numCols = nColumns;
    S->pData = pData;
}
