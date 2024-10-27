#include "memory.h"
#include "file_handling.h"
#include "replacement.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "types.h"

#include "globals.h"
#include "statistics.h"

// initializes the free page list with all physical pages
void initialize_free_page_list() {
    free_page_list = NULL;
    for(int i = MAIN_MEMORY_PAGES -1; i >=0; i--) {
        FreePageList* new_page = (FreePageList*)malloc(sizeof(FreePageList));
        if(!new_page) {
            fprintf(stderr, "Memory allocation failed for FreePageList\n");
            exit(EXIT_FAILURE);
        }
        new_page->page_number = i;
        new_page->next = free_page_list;
        free_page_list = new_page;
    }
}

// initializes the memory map by setting all pages to NULL
void initialize_memory_map() {
    for(int i = 0; i < MAIN_MEMORY_PAGES; i++) {
        memory_map.pages[i] = NULL;
    }
    // initialize FIFO replacement info
    replacement_info.fifo_counter = 0;
    for(int i = 0; i < MAIN_MEMORY_PAGES; i++) {
        replacement_info.fifo_order[i] = -1;
    }
}

// checks if a page is in memory and returns its physical page number if present
bool is_page_in_memory(Process* proc, int virtual_page, int* physical_page) {
    if(virtual_page < 0 || virtual_page >= proc->size_pages) { return false;}
    if(proc->page_table[virtual_page] != -1) {
        *physical_page = proc->page_table[virtual_page];
        return true;
    }
    return false;
}

// updates the access time and count for a page
void update_page_access(int physical_page, int current_time) {
    if(physical_page < 0 || physical_page >= MAIN_MEMORY_PAGES) return;
    if(memory_map.pages[physical_page] != NULL) {
        memory_map.pages[physical_page]->last_access_time = current_time;
        memory_map.pages[physical_page]->access_count += 1;
    }
}

// swaps a page into memory, evicting a victim if necessary
void swap_page_in(Process* proc, int virtual_page, int current_time, Statistics* stats, FILE* fp, Process* Q) {
    // check if a free page is available
    int physical_page = -1;
    if(free_page_list != NULL) {
        physical_page = free_page_list->page_number;
        // remove from free page list
        FreePageList* free_page = free_page_list;
        free_page_list = free_page_list->next;
        free(free_page);
    } else {
        // need to evict a page based on replacement algorithm
        physical_page = select_victim_page();
        if(physical_page == -1) {
            fprintf(fp, "No page available to evict!\n");
            return;
        }

        // evict the victim page
        Page* victim_page = memory_map.pages[physical_page];
        if(victim_page != NULL) {
            // find the victim process by iterating through the current simulation's job queue
            Process* victim_proc = NULL;
            for(int i=0;i< TOTAL_PROCESSES; i++) {
                if(strcmp(Q[i].name, victim_page->process_name) == 0) {
                    victim_proc = &Q[i];
                    break;
                }
            }

            if(victim_proc != NULL) {
                // update victim's page table
                for(int vp = 0; vp < victim_proc->size_pages; vp++) {
                    if(victim_proc->page_table[vp] == physical_page) {
                        victim_proc->page_table[vp] = -1;
                        break;
                    }
                } 
            }
            // log the eviction
            fprintf(fp, "[Time %d.%03d] Evicting Page %d of Process %s from Physical Page %d using %s Replacement\n",
                    current_time / 1000, current_time % 1000,
                    victim_page->virtual_page_number, victim_page->process_name,
                    physical_page,
                    (current_algorithm == FIFO) ? "FIFO" :
                        (current_algorithm == LRU) ? "LRU" :
                        (current_algorithm == LFU) ? "LFU" :
                        (current_algorithm == MFU) ? "MFU" : "Random");

            // remove the victim page from memory
            free(victim_page);
            memory_map.pages[physical_page] = NULL;
        }
    }
    // assign the physical page to the process
    proc->page_table[virtual_page] = physical_page;

    // allocate and assign Page structure
    Page* new_page = (Page*) malloc(sizeof(Page));
    if(!new_page) {
        fprintf(fp, "Memory allocation failed for new Page\n");
        exit(EXIT_FAILURE);
    }
    strcpy(new_page->process_name, proc->name);
    new_page->page_number = physical_page;
    new_page->virtual_page_number = virtual_page;
    new_page->last_access_time = current_time;
    new_page->access_count = 1;
    memory_map.pages[physical_page] = new_page;

    // update replacement info for FIFO
    if(current_algorithm == FIFO) {
        replacement_info.fifo_order[physical_page] = replacement_info.fifo_counter++;
    }

    // log the page swap-in
    fprintf(fp, "[Time %d.%03d] Process %s swapped in Page %d to Physical Page %d\n",
            current_time / 1000, current_time % 1000,
            proc->name, virtual_page, physical_page);
    print_memory_map(fp);

    // update statistics
    stats->processes_swapped_in++;
}

// handles a page fault by swapping in the required page
void handle_page_fault(Process* proc, int virtual_page, int current_time, Statistics* stats, FILE* fp, Process* Q) {
    swap_page_in(proc, virtual_page, current_time, stats, fp, Q);
}

// releases all pages occupied by a process and frees memory
void release_pages(Process* proc) {
    for(int i = 0; i < proc->size_pages; i++) {
        if(proc->page_table[i] != -1) {
            int physical_page = proc->page_table[i];
            // remove the page from memory map
            if(memory_map.pages[physical_page] != NULL) {
                free(memory_map.pages[physical_page]);
                memory_map.pages[physical_page] = NULL;
            }

            // add the physical page back to free list
            FreePageList* new_free = (FreePageList*)malloc(sizeof(FreePageList));
            if(!new_free) {
                fprintf(stderr, "Memory allocation failed for FreePageList during release\n");
                exit(EXIT_FAILURE);
            }
            new_free->page_number = physical_page;
            new_free->next = free_page_list;
            free_page_list = new_free;

            // reset the page table entry
            proc->page_table[i] = -1;
        }
    }
    // free the page table
    free(proc->page_table);
}

