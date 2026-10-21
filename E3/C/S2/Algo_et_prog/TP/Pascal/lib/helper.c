#include "helper.h"

void triangle_pascal_s(uint32_t arg_matrice_triangle_pascal_s[][RANG_STATIQUE_j_y], uint8_t rang)
{
    for (int y = 0; y < rang; y++)
    {
        arg_matrice_triangle_pascal_s[0][y] = 1;
        printf("1 ");
        for (int x = 1; x <= y; x++)
        {
            uint32_t pas_next = 0;
            uint32_t pas_prev = arg_matrice_triangle_pascal_s[x - 1][y - 1];
            if (x < y)
            {
                pas_next = arg_matrice_triangle_pascal_s[x][y - 1];
            }
            arg_matrice_triangle_pascal_s[x][y] = pas_prev + pas_next;
            printf("%d ", arg_matrice_triangle_pascal_s[x][y]);
        }
        printf("\n");
    }
}

void triangle_pascal_dyn(uint64_t **matrice, uint8_t rang)
{
    for (int y = 0; y <= rang; y++)
    {
        matrice[0][y] = 1;
        printf("1 ");
        for (int x = 1; x <= y; x++)
        {
            uint64_t pas_next = 0;
            uint64_t pas_prev = matrice[x - 1][y - 1];

            if (x < y)
            {
                pas_next = matrice[x][y - 1];
            }

            matrice[x][y] = pas_prev + pas_next;

            printf("%4d ", matrice[x][y]);
        }
        printf("\n");
    }
}