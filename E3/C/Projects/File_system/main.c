#include "stdio.h"
#include "core.h"
#include <time.h>


int main(int argc, char *argv[]){
    if (argc < 2)
    {
        display_help();
        return 1;
    }
    int selected_mode = atoi(argv[1]);

    FileMetadata Filemeta;
    init_metadata(&Filemeta);

    switch (selected_mode)
    {
    case HELP:
    printf("HELP");
        display_help();
            break;
    case HISTOGRAM:
        process_file_stats(argv[2], &Filemeta);
        printf("Name : %s, Size : %lu, Inode %lu \n", Filemeta.filename, Filemeta.file_size, Filemeta.inode_number);
        printf("||DEC | HEX | Char ||\n");
        for (uint16_t i = 0; i <= 255; i++)
        {
            if (Filemeta.frequency_counter[i] != 0){
                if (i == '\n')
                {
                    printf("|| Code: %u | Char: \\n | Compteur: %u ||\n", i, Filemeta.frequency_counter[i]);
                }
                else if (i == ' ')
                {
                    printf("|| Code: %u | Char: SPACE | Compteur: %u ||\n", i, Filemeta.frequency_counter[i]);
                }
                else
                {

                    printf("|| Code: %u | Char: \"%c\" | Compteur: %u ||\n", i, i, Filemeta.frequency_counter[i]);
                }
             }
        }
        break;
    case REVERSE:
        reverse_file_copy(argv[2], argv[3]);
        break;
    case REPORT:
        process_file_stats(argv[2], &Filemeta);
        generate_file_report(argv[3], &Filemeta);
        break;
    case SUM:
    {
        int sumtablesize = argc - 3;
        int *sumtable = malloc(sizeof(int) * (sumtablesize));
        for (int i = 0; i < sumtablesize; i++)
        {
            sumtable[i] = atoi(argv[i+3]);
        }
        printf("SUM : %ld \n",sum(sumtablesize,argv[2],sumtable));
        free(sumtable);
    }
        break;
    case MULTITABLE:
    {
        uint8_t val_multitable = atoi(argv[2]);
        multitable(val_multitable);
    }
        break;
    case LARGEST:
        largestfile(argc - 2, (const char **)&argv[2]);
        break;
    case LASTLINE:
        last_line(argv[2]);
            break;
        case LOSTFILE:
            break;
        default:
            break;
        }

    return 0;
}