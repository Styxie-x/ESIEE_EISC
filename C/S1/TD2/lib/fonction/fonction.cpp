
#include "fonction/fonction.h"

void exo1() {
        printf("Exercice 1\n");
        Resdiv X = euclide(9,2);
        printf("9/2 => Q = %d R = %d \n", X.Q, X.R);

        X = euclide(9,-2);
        printf("9/-2 => Q = %d R = %d \n", X.Q, X.R);
        
        X = euclide(9,0);
        printf("9/0 => Q = %d R = %d \n", X.Q, X.R);
        
}

Resdiv euclide(int x, int y){
        if(y==0) 
                return {-1,-1};
        return {x/y, x%y};
}


void exo2(){
    affiche(paire, 9);
    affiche(impair, 10);
    affiche(paire, 0);
}

void affiche(partie p, int N){
    
    if (N < 0) {
        printf("(.............error............;)\n");
        return;
    }
    Resdiv x = euclide(N,2);
    if (x.R == paire) {
        for (int i = 0; i <= N; i += 2) {
            printf("%d ", i);
        }
    } else if (x.R == impair) {
        for (int i = 1; i <= N; i += 2) {
            printf("%d ", i);
        }
    } else {
        for (int i = 0; i <= N; ++i) {
            printf("%d ", i);
        }
    }
    printf("\n");
}