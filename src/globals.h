#ifndef GLOBALS_H
#define GLOBALS_H

#include "types.h"

// global Variables
extern FreePageList* free_page_list;
extern JobQueue job_queue;
extern MemoryMap memory_map;
extern ReplacementAlgorithm current_algorithm;
extern ReplacementInfo replacement_info;
extern JobQueue* current_sim_job_queue;

#endif // gLOBALS_H
