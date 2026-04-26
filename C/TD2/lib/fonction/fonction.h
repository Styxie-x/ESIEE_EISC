#ifndef FONCTION_H
#define FONCTION_H

#include <time.h>

#include <iostream> // input stream and output stream objects, e.g. std::cout, std::cin.
#include <stdio.h>  // input/output functionalities through the use of printf() and scanf().


void exo1();

typedef struct Resdiv{
   int Q; // Quotient
   int R; // Reste 
} Resdiv;

Resdiv euclide(int x, int y);

typedef enum{
   paire,
   impair
}partie;

void exo2();
void affiche(partie p, int N);



#endif