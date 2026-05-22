#include "fonction/fonction.h"


void exo1(){
    int n = 123456789;
    int inv_n = imprime_inverse(n);
    printf("Le nombre inverse de %d est %d\n", n, inv_n);

}
int imprime_inverse(int n){
    int i_n = 0;
    while(n != 0){
        i_n = (i_n * 10) + (n % 10);
        n = n / 10;
    }
    return i_n;
}

void exo2(){
    int a = 5;
    int N = 5;
    printf("%f \n",pow_N(a,N));
    printf("%f \n",pow_R(a,N));
}

float pow_N(float a, int N){
    float pow = 1;

    for(int i =0; i<N; i++){
        pow = pow * a;
    }
    return pow;
}

float pow_R(float a, int N){

    int i = 0;
    float pow = 1;
    if(N == 0) return 1; 
    return a * pow_R(a, N-1);

}
void exo3() {
    FILE *f = fopen("copie.txt", "w");
    if (f == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }
    
    printf("Entrez une chaîne de caractères : ");
    fgetc(stdin); // Clear the newline character from the input buffer
    char c;
    
    while ((c = fgetc(stdin)) != '\n' && c != EOF) {
        fputc(c, f);
    }
    
    fclose(f);
    putc('\n', stdout);
}


