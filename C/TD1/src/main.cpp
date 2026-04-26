// #include <pthread.h>
// #include "../lib/Module_1/Module_1.h"  // Adjust the relative path
// #include "../lib/Module_2/Module_2.h"  // Adjust the relative path

#include <iostream> // nput stream and output stream objects, e.g. std::cout, std::cin.
#include <stdio.h>  // input/output functionalities through the use of printf() and scanf().
#include <stdlib.h> // general purpose functions, e.g. memory allocation, process control, conversions, etc.
#include <math.h>
/* --------------------- DECLARATION --------------------- */

void exo1();

void exo2();
int exo2_somme_if(int N);
int exo2_somme_for(int N);
int exo2_somme_while(int N);

void exo3();
int exo_3_fact_R(int N);
int exo_3_fact_N(int N);

void exo4();
float exo4_power_N(float x, int n);
float exo4_power_R(float x, int n);

void exo5();
void exo5_print_array(int tableau[], int n);

void exo6();
int pgcd(int a, int b);



/* --------------------- MAIN --------------------- */

int main() {

  
  printf("TD1 !\n");
  printf ("-------------Exercice 1 !-------------\n");
  exo1();

  fflush(stdout); // Will now print everything in the stdout buffer
  printf ("-------------Exercice 2 !-------------\n");

  exo2();

  printf ("-------------Exercice 3 !-------------\n");

  exo3();

  printf ("-------------Exercice 4 !-------------\n");

  exo4();

  printf ("-------------Exercice 5 !-------------\n");
  exo5();

  printf ("-------------Exercice 6 !-------------\n");
  exo6();

 // while (1) {    }
  return 0;
}

/* --------------------- INITIALISATION --------------------- */

/* ________________________ EXO_1 ____________________________ */


void exo1() {
  const int a = 255;
  const float b = 3.14;
  printf("valeur de a est : %d Taille est :  %zu octets \n", a, sizeof(a));
  printf("valeur de a est : 0x%x Taille est :  0x%x \n", a, sizeof(a));
  printf("valeur de b est : %f Taille est :  %zu octets \n", b, sizeof(b));
  printf("valeur de b est : 0x%x Taille est :  0x%x \n", *(int*)&b, sizeof(b));
}

/* ________________________ EXO_2 ____________________________ */

void exo2() {
  const int N = 100;
  printf("somme if de %d est = %d\n", N, exo2_somme_if(N));
  printf("somme for de %d est = %d\n", N, exo2_somme_for(N));
  printf("somme while de %d est = %d\n", N, exo2_somme_while(N));
}

int exo2_somme_if(int N){
  int somme = 0;
  if (N > 0) {
    somme = N + exo2_somme_if(N - 1);
  }
  return somme;
}


int exo2_somme_for(int N){
  int somme = 0;
  for(int i = 0; i <= N; i++) {
    somme += i;
  }
  return somme;
}

int exo2_somme_while(int N){
  int somme = 0;
  int i = 0;
  while(i <= N) {
    somme += i;
    i++;
  }
  return somme;
}

/* ________________________ EXO_3 ____________________________ */

void exo3()
{
  int N = 3;
  printf("%d \n",exo_3_fact_R(N));
  printf("%d \n",exo_3_fact_N(N));

}

int exo_3_fact_R(int N)
{
  if(N<=1)
  {
    return 1;
  }
  else
  {
    int factorielle = N;
    for(int i = 0; i <= N; i++) {
      factorielle = factorielle * (exo_3_fact_R(i-1));
    }
    return factorielle;
  }
  
}

int exo_3_fact_N(int N)
{
  if(N<=1)
  {
    return 1;
  }

  int i;
  int factorielle = N;
  i = N-1;
  do{
    factorielle = factorielle * i;
    i--;
  }while(i>1);
  return factorielle;

  /*
  
  ----------- les 2 methodes fonnctionnent pour faire factorielle non recursive -----------
  */
  
  // x:
  // if (i==0)
  // {
  //   return factorielle;
  // }
  // factorielle = factorielle * i;  
  // i--;
  // goto x;  
}

/* ________________________ EXO_4 ____________________________ */


void exo4()
{
  float x = 2.0;
  int n = 2;
  printf( " %f \n ",  pow(x,n));
  printf( " %f \n", exo4_power_N(x, n));

}


float exo4_power_N(float x, int n)
{
  float pow = 0.0;
  pow = x;
  int i = n-1;

  if(i<0)
  {
    printf("ERREUR : N<0");
    return 0;
  }else{
    do{
      pow = pow * x;
      i--;

    }while(i>0);
    return pow;
  }
}

float exo4_power_R(float x, int n)
{
  if(n<0)
  {
    printf("ERREUR : N<0");
    return 0;
  }else{

    
    // @TODO Implement the recursive power function
    return 0;
  }
}

/* ________________________ EXO_5 ____________________________ */

void exo5()
{
  int longeur = 100;
  int tab[longeur];
  for(int i = 0; i<longeur; i++)
  {
    tab[i]=i;
  }
  exo5_print_array(tab, longeur);
}

void exo5_print_array(int tableau[], int n)
{
  for(int i = 0; i < n; i++)
  {
    printf(" %d ", tableau[i]);
  }
  printf("\n");
}

/* ________________________ EXO_6 ____________________________ */

void exo6()
{
  int a = 35;
  int b = 15;
  printf("Le PGCD de %d et %d est : %d\n", a, b, pgcd(a, b));
}

int pgcd(int a, int b)
{
  if(a==0)
    return b;
  return pgcd(b % a, a);
}