
#include "fonction/fonction.h"

void exo1(){
    printf("\n ***** EXO_1 *****\n");
    int N = 0;
    N = compteur();
    for(int i = 0; i<=10;i++){
        N = compteur();
        printf("%d ",i);
    }
    printf(" | There were %d compteur calls\n", N);

    for(int i = 0; i<=10;i++){
        N = compteur();
        printf("%d ",i);
    }

    printf(" | There were %d compteur calls\n", N);
}

int compteur(){
    static int calls = 0;
    
    return calls++;
}

void exo2(){
    printf("\n***** EXO_2 *****\n");
    int Q,R;
    int *ptrQ = &Q;
    int *ptrR = &R;
    
    //int x, y = 3, 9;
    int x,y;
    x = 3, y = 9;
    euclidienl(x, y, ptrQ, ptrR);
    printf("\n %d/%d : Q = %d et R = %d", x, y, Q, R);

    x = 9, y = 2;
    euclidienl(x, y, ptrQ, ptrR);
    printf("\n %d/%d : Q = %d et R = %d", x, y, Q, R);

    x = 9, y = 0;
    euclidienl(x, y, ptrQ, ptrR);
    printf("\n %d/%d : Q = %d et R = %d", x, y, Q, R);

    printf("\n");


}


int euclidienl(int x, int y, int* Q, int* R ){
    int q,r;    
    if(y==0){
        *R = -1;
        *Q = -1; 
        return -1;
    }
        
    q = x/y;
    *Q = q;
    r = x%y;
    *R = r;
    return 0;
}


void exo3(){
    int a,b;
    a = 3, b = 9;
    int *ptr_a = &a;
    int *ptr_b = &b;

    printf("\n***** EXO_3 *****\n");
    printf("a = %d , b = %d \n", a, b);
    printf("inverser a et b\n");
    invert_var(ptr_a, ptr_b);
    printf("a = %d , b = %d \n", a, b);
     
    
}

void invert_var(int* a, int* b){
    int c = 0;
    c = *a;
    *a = *b;
    *b = c;
}


