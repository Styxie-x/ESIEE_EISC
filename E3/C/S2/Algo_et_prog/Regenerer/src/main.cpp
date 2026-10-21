#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* --------------------- DECLARATION --------------------- */
int search_in_table(int tab[], int start, int end, int search);

/* --------------------- MAIN --------------------- */
int main()
{
  printf("------------Search------------\n\n\n");

  int mon_tableau[] = {2, 5, 8, 12, 16, 23, 38, 56, 72, 91};
  int taille = sizeof(mon_tableau) / sizeof(mon_tableau[0]);
  int debut = 0;
  int fin = taille - 1;
  int cible = 23;

  int resultat = search_in_table(mon_tableau, debut, fin, cible);

  if (resultat != -1)
  {
    printf("Élément trouvé à l'index : %d\n", resultat);
  }
  else
  {
    printf("Élément non trouvé dans le tableau.\n");
  }

  return 0;
}

/* --------------------- FUNCTIONS --------------------- */
int search_in_table(int tab[], int start, int end, int search)
{
  if (start > end)
  {
    return -1;
  }

  int middle = start + (end - start) / 2;

  if (tab[middle] == search)
  {
    return middle;
  }
  if (tab[middle] > search)
  {
    return search_in_table(tab, start, middle - 1, search);
  }

  return search_in_table(tab, middle + 1, end, search);
}