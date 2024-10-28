#include <stdlib.h>
#include <stdio.h>
#include "replacement.h"
#include "globals.h"

int select_victim_page_fifo() {
    // printf("FIFO");
    int fifo_page = 0, least_fifo_counter = replacement_info.fifo_order[0];
    for(int i = 1; i < MAIN_MEMORY_PAGES; i++) {
        if(replacement_info.fifo_order[i] < least_fifo_counter) {
            least_fifo_counter = replacement_info.fifo_order[i];
            fifo_page = i;
        }
    }
    return fifo_page;
}
int select_victim_page_lru() {
    // printf("LRU");
    int last_recenty_used = 0, lru_access_time = memory_map.pages[0]->last_access_time;
    for(int i = 1; i < MAIN_MEMORY_PAGES; i++) {
        if(memory_map.pages[i]->last_access_time < lru_access_time) {
            lru_access_time = memory_map.pages[i]->last_access_time;
            last_recenty_used = i;
        }
    }
    return last_recenty_used;
}
int select_victim_page_lfu() {
    // printf("LFU");
    int last_least_used = 0, lfu_access_count = memory_map.pages[0]->access_count;
    for(int i = 1; i < MAIN_MEMORY_PAGES; i++) {
        if(memory_map.pages[i]->access_count < lfu_access_count) {
            lfu_access_count = memory_map.pages[i]->access_count;
            last_least_used = i;
        }
    }
    return last_least_used;
}
int select_victim_page_mfu() {
    //  printf("MFU");
    // printf("Selecting victim using MFU\n");
    int last_most_used = 0, mfu_access_count = memory_map.pages[0]->access_count;
    for(int i = 1; i < MAIN_MEMORY_PAGES; i++) {
        if(memory_map.pages[i]->access_count > mfu_access_count) {
            mfu_access_count = memory_map.pages[i]->access_count;
            last_most_used = i;
        }
    }
    return last_most_used;
}
int select_victim_page_random() {
    // printf("Random");
    return rand() % MAIN_MEMORY_PAGES;
}
int select_victim_page() {
    switch(current_algorithm) {
        case FIFO:
            return select_victim_page_fifo();
        case LRU:
            return select_victim_page_lru();
        case LFU:
            return select_victim_page_lfu();
        case MFU:
            return select_victim_page_mfu();
        case RANDOM_REPL:
            return select_victim_page_random();
        default:
            return -1;
    }
}