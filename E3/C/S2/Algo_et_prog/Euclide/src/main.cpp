// #include <pthread.h>
// #include "../lib/Module_1/Module_1.h"  // Adjust the relative path
// #include "../lib/Module_2/Module_2.h"  // Adjust the relative path

#include <iostream> // input stream and output stream objects, e.g. std::cout, std::cin.
#include <stdio.h>  // input/output functionalities through the use of printf() and scanf().
#include <stdlib.h> // general purpose functions, e.g. memory allocation, process control, conversions, etc.
#include <math.h>
#include "fonction/fonction.h"
/* --------------------- DECLARATION --------------------- */
int pgcd_nr(int a, int b);
int pgcd_r(int a, int b);

/* --------------------- MAIN --------------------- */

int main()
{

  printf("------------Euclide------------\n\n\n");
  printf("%d \n", pgcd(2, 10));

  // while (1){}
  return 0;
}

int pgcd(int a, int b)
{
  if (b == 0)
  {
    return 0;
  }
  int res = 1;
  while (b > 0)
  {
    res = a % b;
    a = b;
    b = res;
  }
  return a;
}

int pgcd_r(int a, int b)
{
  if (b == 0)
  {
    return 0;
  }
  return pgcd(b, a % b);
}

/* --------------------- FUNCTIONS --------------------- */