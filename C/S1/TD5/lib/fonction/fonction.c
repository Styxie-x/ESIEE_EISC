#include "fonction/fonction.h"

void exo1(){
    printf("exo1 output,: \n");

    char a[11];
    for(uint8_t i = 0; i<10; i++){
        a[i] = '0' + i;
    }
        a[10] = '\0'; // Null-terminate the string

    for(uint8_t i = 0; i<10; i++){
        printf("%c, ",a[i]);
    }
    printf("\n");
}

void exo2(){
    printf("exo2 output :\n");
    char a[7];
    char *b;
    char * c = "Hi";
    char d[] = "Hi";
    
    a[0] = 'H';
    a[1] = 'i';
    a[2] = '\0';
    for(uint8_t i = 3; i<7;i++){
        a[i] = '\0';
    }
    
    b = (char *)malloc(sizeof(char)*3);
    b[0] = 'H';
    b[1] = 'i';
    b[2] = '\0';

    printf(" a = %s,\n b = %s,\n c = %s,\n d = %s;\n", a,b,c,d);

}

void exo3(){

    printf("exo3 output\n");
    const char str[] = "Hello";
    printf(" strlen = %d \n", my_strlen(str));  
}

int my_strlen(const char *c){
    int i = 0;
    while(c[i] != '\0')
    {
        i++;
    }
    return i;
    
}

void exo4(){
    printf("exo4 output :\n");
    char str[] = "Hello my    str  ing   with spaces";
    printf("%s",my_squeeze_string(str));
}

char * my_squeeze_string(const char *c){
    int i_sq = 0;
    int i_c = 0;
    char *sq_str = 0;
    int size_c = my_strlen(c);
    
    sq_str = (char *)malloc(size_c*sizeof(char));

    while(c[i_sq] != '\0')
    {
        if(c[i_c] !=' '){
            sq_str[i_sq] = c[i_c];
            i_sq++;
        }
        i_c++;
    }
    sq_str[i_sq] = '\0';
    return sq_str;
}

void exo6() {
    FILE *f = fopen("fichier1.txt", "w");
    if (f == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }
    const char *texte = "Ceci est une chaîne écrite dans le fichier.";
    fprintf(f, "%s", texte);
    fclose(f);
    printf("Chaîne écrite dans fichier1.txt\n");
}

