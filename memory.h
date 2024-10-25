#ifndef MEMORY_H
#define MEMORY_H

#include "types.h"
#include <stdio.h>

// memory Management Functions
void initialize_free_page_list();
void initialize_memory_map();
bool is_page_in_memory(Process* proc, int virtual_page, int* physical_page);
void update_page_access(int physical_page, int current_time);
void swap_page_in(Process* proc, int virtual_page, int current_time, Statistics* stats, FILE* fp);
void handle_page_fault(Process* proc, int virtual_page, int current_time, Statistics* stats, FILE* fp);
void release_pages(Process* proc);

#endif // mEMORY_H
