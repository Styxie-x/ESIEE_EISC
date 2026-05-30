#include "../lib/helper.h"
#include <stdlib.h>

int main()
{
    printf("------------------start-------------------\n");
    printf("\n tableau statique : \n");
    uint32_t matrice_triangle_pascal_s[RANG_STATIQUE_i_x][RANG_STATIQUE_j_y];
    triangle_pascal_s(matrice_triangle_pascal_s, 5);

    printf("\n------------------Dynamique-------------------\n");

    printf("tableau dynamique = ");
    uint8_t rang_dynamique = 0;
    scanf("%hhu", &rang_dynamique); // hhu pour 1 octey
    uint64_t **matrice_triangle_pascal_dyn = malloc(rang_dynamique * sizeof(uint64_t *));
    if (matrice_triangle_pascal_dyn == NULL)
    {
        printf("failed to allocate memory");
    }
    for (int i = 0; i <= rang_dynamique; i++)
    {
        matrice_triangle_pascal_dyn[i] = malloc((i + 1) * sizeof(uint64_t));
    }

    triangle_pascal_dyn(matrice_triangle_pascal_dyn, rang_dynamique);

    for (int i = 0; i <= rang_dynamique; i++)
    {
        free(matrice_triangle_pascal_dyn[i]);
    }

    free(matrice_triangle_pascal_dyn);

    printf("\n------------------end-------------------\n");
    return 0;
}