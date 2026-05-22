#include "inc/vfs.h"

Node* Create_Node(const char* arg_p_name, NodeType arg_type, Node* arg_p_parent)
{
    char* p_name = (char*)mallo(sizeof(arg_p_name));
    Node* p_parent = 
    Node* NEW_Node = (Node*)malloc(sizeof(Node));
    if(NEW_Node == NULL){
        printf(" Failed to allocate NEW_Node");
    }

    NEW_Node->name = name;
    NEW_Node->type = type;
    NEW_Node->parent = parent;
    /*
    uint64_t count_children;
    uint64_t current_allocated_mem_children_count;

    struct Node** childrens;
    uint64_t size;
    char* content;
    */
}