#include "core.h"
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>

    const char *const ModeName[MODECOUNT] = {
        "HELP",
        "HISTOGRAM ",
        "REVERSE ",
        "REPORT ",
        "SUM ",
        "MULTITABLE ",
        "LARGEST ",
        "LASTLINE ",
        "LOSTFILE"};

void display_help(){
    printf("USAGE\n");
    printf("Please provide one of following arguments\n");

    for (uint8_t index = 0; index <= MODECOUNT - 1; index++)
    {
        printf("%d : Mode : %s \n", index, ModeName[index]);
    }
}

void init_metadata(FileMetadata *arg_meta)
{
    memset(arg_meta, 0, sizeof(FileMetadata));
}

int process_file_stats(const char *path, FileMetadata *arg_meta)
{
    strncpy((*arg_meta).filename, path, 128*sizeof(char));
    int fd = open((*arg_meta).filename,O_RDONLY);
    if(fd == -1){
        return -1;
    }
    struct stat meta_stat;
    unsigned char buffer[1024];
    uint64_t totalbytes = 0;

    while (1)
    {
        ssize_t read_byte = read(fd,buffer,sizeof(buffer));
        if (read_byte == 0)
        {
            break;
        } else{
           totalbytes = totalbytes + read_byte; 
        }
        for(ssize_t i = 0; i< read_byte; i++)
        {
            unsigned char caractere = buffer[i];
            (*arg_meta).frequency_counter[caractere]++;
        }
    }
    (*arg_meta).file_size = totalbytes;
    fstat(fd,&meta_stat);
    (*arg_meta).inode_number = meta_stat.st_ino;
    close(fd);
    return 0;
}


int reverse_file_copy(const char *src_path, const char *dest_path){

    int src_fd = open(src_path, O_RDONLY);
    if (src_fd == -1)
    {
        return -1;
    }

    int des_fd = open(dest_path, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (des_fd == -1)
    {
        return -1;
    }

    unsigned char *buffer;
    off_t sizebuffer = lseek(src_fd, 0, SEEK_END);
    lseek(src_fd, 0, SEEK_SET);
    buffer = malloc(sizebuffer );
    if (buffer == NULL)
    {
        printf("reverse_file_copy() buffer : failed to allocate memory");
        return 1;
    }
    read(src_fd, buffer, sizebuffer);
    for (off_t i = sizebuffer - 1; i >= 0; i--)
    {
        write(des_fd, &buffer[i], 1);
    }
    free(buffer);
    close(des_fd);
    close(src_fd);
    return 0;
}

int generate_file_report(const char *report_path, const FileMetadata *arg_meta){
    int report_fd = open(report_path, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (report_fd == -1)
    {
        return -1;
    }

    char buffer[255];
    memset(&buffer,0,255);
    
    snprintf(buffer,255,"Report \n Name : %s, Size = %lu, Inode = %lu \n",arg_meta->filename, arg_meta->file_size, arg_meta->inode_number);
    write(report_fd, buffer, strlen((char*)buffer));
    close(report_fd);
    return 0;
}

long sum(int countV, const char *sum_path, int *sumarray)
{
    long sum = 0;
    for (int i = 0; i < countV; i++)
    {
        sum = sum + sumarray[i];
    }
    int des_fd = open(sum_path, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (des_fd == -1)
    {
        return -1;
    }
    char *buffer;
    off_t sizebuffer = 1024;

    buffer = malloc(sizebuffer);
    if (buffer == NULL)
    {
        printf("sum() buffer : failed to allocate memory");
        return 1;
    }
    memset(buffer, 0, sizebuffer);
    snprintf(buffer, sizebuffer, "SUM =  %ld \n", sum);

    write(des_fd, buffer, strlen((char *)buffer));
    free(buffer);
    close(des_fd);
    return sum;
}

int multitable(uint8_t val){
    if (mkdir("Output", S_IRWXU) == -1)
    {
        if (errno != EEXIST)
        {
            perror("Failed to create Output directory");
            return -1;
        }
    }
    int multiply = 0;
    char buffer[1024];
    memset(buffer,0,1024);
    char *ptr = buffer;
    int total_written = 0;
    for(uint8_t i = 0; i <= 10; i++)
    {
        multiply = val * i;
        int step = snprintf(ptr, 1024 - total_written, "%d x %d = %d \n", val, i, multiply);
        ptr += step;
        total_written += step;
    }
    int des_fd = open("./Output/multitable.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (des_fd == -1)
    {
        return -1;
    }
    write(des_fd, buffer, strlen((char *)buffer));
    return 0;
}

int largestfile(int argc, const char *argv[]){
    struct stat stats;
    int largest_file_index = 0;
    off_t largest_file_size = 0;
    for (int i = 0; i < argc; i++)
    {
        if((stat(argv[i],&stats)) == 0)
        {
            if(stats.st_size >= largest_file_size)
            {
                largest_file_size = stats.st_size;
                largest_file_index = i;
            }
        }
    }
    printf("Largest File = %s with size of : %lu bytes\n", argv[largest_file_index], largest_file_size);
    return 0;
}

int last_line(const char *path)
{
    int fd = open(path, O_RDONLY);
    if (fd == -1)
    {
        return -1;
    }

    lseek(fd, 0, SEEK_END);
    char caracter_read = 0;
    uint16_t buffersize = 0;
    uint8_t Nextlinecount = 0;
    lseek(fd, -1, SEEK_CUR);

    char *buffer = NULL;
    off_t pos = 1; // Initialisé à 1 pour pouvoir entrer et tester dans la boucle

    while (1)
    {
        ssize_t read_byte = read(fd, &caracter_read, 1);
        if (read_byte <= 0)
        {
            break; // Sécurité si le fichier est vide ou erreur de lecture
        }

        if (caracter_read == '\n')
        {
            Nextlinecount++;
        }

        if (Nextlinecount == 2)
        {
            buffer = malloc((buffersize + 1) * sizeof(char));
            if (buffer == NULL)
            {
                printf("failed to allocate memory last line\n");
                close(fd);
                return 1;
            }
            lseek(fd, 1, SEEK_CUR); // Saute le \n pour démarrer la lecture du texte
            read(fd, buffer, buffersize);
            buffer[buffersize] = '\0';
            break;
        }

        pos = lseek(fd, -2, SEEK_CUR); // Recule d'un pas net
        if (pos <= 0)
        {
            break; // On a atteint le début du fichier
        }
        buffersize++;
    }

    // Gestion du cas où le fichier n'a qu'une seule ligne (on est remonté jusqu'au début)
    if (buffer == NULL && pos <= 0 && buffersize > 0)
    {
        lseek(fd, 0, SEEK_SET);                           // On se repositionne au tout début du fichier
        buffer = malloc((buffersize + 2) * sizeof(char)); // Assure l'espace nécessaire
        if (buffer != NULL)
        {
            ssize_t bytes_read = read(fd, buffer, buffersize + 1);
            if (bytes_read >= 0)
            {
                buffer[bytes_read] = '\0';
            }
        }
    }

    // Fermeture du fichier unique pour tous les cas de figure
    close(fd);

    if (buffer != NULL)
    {
        printf(" [%s] \n", buffer);
        free(buffer);
    }

    return 0;
}