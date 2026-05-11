// #include <pthread.h>
// #include "../lib/Module_1/Module_1.h"  // Adjust the relative path
// #include "../lib/Module_2/Module_2.h"  // Adjust the relative path

#include <iostream> // input stream and output stream objects, e.g. std::cout, std::cin.
#include <stdio.h>  // input/output functionalities through the use of printf() and scanf().
#include <stdlib.h> // general purpose functions, e.g. memory allocation, process control, conversions, etc.
#include <math.h>
#include "fonction/fonction.h"


#include <sys/types.h>
#include <dirent.h>
#include <stddef.h>
#include <string.h>
#include <stdint.h>

/* --------------------- DECLARATION --------------------- */



/* --------------------- MAIN --------------------- */


int main(int argc, char* argv[]) {

  printf("===================START====================\n");    if (argc < 2) {
        fprintf(stderr, "No argument given\n");
        printf("===================END====================\n");
        return 1;
    }
    char *dirname = argv[1];
    DIR *dir = opendir(dirname);
    if (dir == NULL) {
        fprintf(stderr, "Cannot open directory\n");
        printf("===================END====================\n");
        return 2;
    }
    struct dirent *entry = readdir(dir);
    
    struct dirent *longest_name = readdir(dir);


    uint8_t max_size = 0;
    
    while (entry != NULL){
        printf("%s", entry->d_name);
        printf("the size of %s is %d and the length is %d\n", entry->d_name, sizeof(entry->d_name)/8, strlen(entry->d_name));
        if (strlen(entry->d_name) > max_size) {
            max_size = strlen(entry->d_name);
            strcpy(longest_name->d_name, entry->d_name); 
        }
        entry = readdir(dir);
    }
    printf("================================= \n max size is %d, %s  \n =================================\r\n", max_size, longest_name->d_name);

    closedir(dir);

    printf("===================END====================\n");
    return 0;
}


/* --------------------- FUNCTIONS --------------------- */

