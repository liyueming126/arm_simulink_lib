#include <stdint.h>

typedef int16_t q15_t;

typedef struct {
    uint16_t numRows;
    uint16_t numCols;
    q15_t *pData;
} arm_matrix_instance_q15;

void arm_mat_init_q15(
    arm_matrix_instance_q15 *S,
    uint16_t nRows,
    uint16_t nColumns,
    q15_t *pData)
{
    S->numRows = nRows;
    S->numCols = nColumns;
    S->pData = pData;
}
