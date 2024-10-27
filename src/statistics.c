#include <stdlib.h>
#include <stdio.h>
#include "statistics.h"

void initialize_statistics(Statistics* stats){
    stats->hit_count = 0;
    stats->miss_count = 0;
    stats->processes_swapped_in = 0;
    stats->total_references = 0;
}