#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h>
#include "constants.h"


typedef enum {
    FIFO,
    LRU,
    LFU,
    MFU,
    RANDOM_REPL,
    TOTAL_ALGOS //what is this for?
} ReplacementAlgorithm;

typedef struct Page {
    int page_number;               // physical page number
    char process_name[4];          // process name (e.g., "a0", "b1")
    int virtual_page_number;       // virtual page number within the process
    int last_access_time;          // for LRU
    int access_count;              // for LFU/MFU
} Page;

typedef struct Process {
    char name[4];                  // process name (e.g., "a0", "b1")
    int size_mb;                   // size in MB
    int size_pages;                // size in pages
    int arrival_time;              // arrival time in seconds
    int service_duration;          // service duration in seconds
    int remaining_time;            // remaining service time in seconds
    int* page_table;               // virtual page number to physical page mapping (-1 indicates not in memory)
    int current_page;              // current page being referenced
    struct Process* next;
} Process;

typedef struct JobQueue {
    Process* head;
} JobQueue;

typedef struct FreePageList {
    int page_number;
    struct FreePageList* next;
} FreePageList;

typedef struct MemoryMap {
    Page* pages[MAIN_MEMORY_PAGES];
} MemoryMap;

typedef struct Statistics {
    int hit_count;
    int miss_count;
    int processes_swapped_in;
    int total_references; // tracks total page references
} Statistics;

// replacement Info for FIFO
typedef struct ReplacementInfo {
    int fifo_counter;
    int fifo_order[MAIN_MEMORY_PAGES];
} ReplacementInfo;

// event Structures for precise timing
typedef struct Event {
    int time_ms;           // in milliseconds
    Process* proc;
    int virtual_page;
    struct Event* next;
} Event;

typedef struct EventQueue {
    Event* head;
} EventQueue;

#endif // tYPES_H
