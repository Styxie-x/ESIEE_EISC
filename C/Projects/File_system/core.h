#ifndef CORE_H
#define CORE_H
#include <unistd.h>
#include "stdint.h"
#include <stdlib.h>

enum ProcessMode
{
    HELP = 0,
    HISTOGRAM = 1,
    REVERSE = 2,
    REPORT = 3,
    SUM = 4,
    MULTITABLE = 5,
    LARGEST = 6,
    LASTLINE = 7,
    LOSTFILE = 8,
    MODECOUNT = 9
};
extern const char *const ModeName[MODECOUNT];


typedef struct FileMetadata
{
    char filename[128];
    uint64_t file_size;
    uint64_t inode_number;
    uint32_t frequency_counter[256];
} FileMetadata;
void init_metadata(FileMetadata *arg_meta);
int process_file_stats(const char *path, FileMetadata *arg_meta);
int reverse_file_copy(const char *src_path, const char *dest_path);
int generate_file_report(const char *report_path, const FileMetadata *arg_meta);
void display_help();
long sum(int countV, const char *sum_path, int *sumarray);
int multitable(uint8_t val);
int largestfile(const int argc, const char *argv[]);
int last_line(const char *path);
#endif
