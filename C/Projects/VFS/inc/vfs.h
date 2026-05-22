// Core file system data models and tree mutations

#ifndef VFS_H
#define VFS_H

#include <stdint.h>

typedef enum NodeType{
    DIR = 1,
    FILE = 2
}NodeType;

typedef struct Node
{
    char Name[64];
    NodeType type;
   
    struct Node* parent; // LSC : Previous
    uint64_t count_children;
    uint64_t current_allocated_mem_children_count;

    struct Node** childrens; // LSC : shouldnt Next = Children be *Node?
    uint64_t size;
    char* content;
}Node;

#endif