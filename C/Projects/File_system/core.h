#ifndef CORE_H
#define CORE_H
#include "stdint.h"

enum ProcessMode
{
    HISTOGRAM = 1,
    REVERSE = 2,
    REPORT = 3
};

typedef struct FileMetadata
{
    char filename[128];
    uint64_t file_size;
    uint64_t inode_number;
    uint32_t frequency_counter[256];
} FIleMetadata;

#endif
