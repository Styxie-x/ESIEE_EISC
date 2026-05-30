#ifndef HELPER_H
#define HELPER_H
#include "stdint.h"
#include "stdio.h"

#define RANG_STATIQUE_i_x 10
#define RANG_STATIQUE_j_y 10

void triangle_pascal_s(uint32_t arg_matrice_triangle_pascal_s[][RANG_STATIQUE_j_y], uint8_t rang);
void triangle_pascal_dyn(uint64_t **matrice, uint8_t rang);

#endif