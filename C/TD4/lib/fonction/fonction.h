#ifndef FONCTION_H
#define FONCTION_H

#include <time.h>

#include <iostream> // input stream and output stream objects, e.g. std::cout, std::cin.
#include <stdio.h>  // input/output functionalities through the use of printf() and scanf().
#include <stdlib.h>
void exo1();
void exo2();
void exo3();
float min(float* p,int N);

void exo4();
float *create_tab(int size, float value);

void exo5();
int *create_double(const int* tab, int N);

void exo6();
float * concat(float* tab1, int n1, float * tab2, int n2);

#endif