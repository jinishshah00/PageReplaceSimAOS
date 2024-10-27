#include "globals.h"

int swaps = 0;
FreePageList* free_page_list = 0;
JobQueue job_queue = { 0 };
MemoryMap memory_map;
ReplacementAlgorithm current_algorithm;
ReplacementInfo replacement_info;
JobQueue* current_sim_job_queue = 0;
