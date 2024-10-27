#include "file_handling.h"
#include "globals.h"
#include "memory.h" 
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>

// creates the outputs directory if it doesn't exist
void create_outputs_directory() {
    struct stat st = {0};
    if (stat("outputs", &st) == -1) {
        if(mkdir("outputs", 0700) != 0) {
            perror("mkdir");
            exit(EXIT_FAILURE);
        }
    }
}

void print_memory_map(FILE* fp) {
    fprintf(fp, "Memory Map:\n");
    for(int i = 0; i < MAIN_MEMORY_PAGES; i++) {
        if(memory_map.pages[i] != NULL)
            fprintf(fp, "%s ", memory_map.pages[i]->process_name);
        else
            fprintf(fp, " . ");

        if ((i + 1) % 10 == 0)
            fprintf(fp, "\n");
    }
    fprintf(fp, "\n");
}