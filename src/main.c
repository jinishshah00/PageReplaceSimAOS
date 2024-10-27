#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "types.h"
#include "globals.h"
#include "constants.h"
#include "memory.h"
#include "statistics.h"
#include "file_handling.h"

// creating a max function
#define MAX(x, y) (((x) > (y)) ? (x) : (y));

// Placeholder for process definition and Total_PROCESS constant
// #define Total_PROCESS 150
// #define PROCss_DuraTN 5 // Maximum process duration

// typedef struct {
//     int pid;          // Process ID
//     int PGECNTER;     // Number of pages (process size in MB)
//     int TIMEARR;      // Arrival time (in seconds)
//     int DRUTION;      // Service duration (in seconds)
//     int PGCRR;        // Current page (starting at 0)
// } process;

// Function to compare arrival times for sorting
int cmp_arrival_time(const void* a, const void* b) {
    return ((Process*)a)->arrival_time - ((Process*)b)->arrival_time;
}

int generate_delta_i(int page_size) {
        int r = rand() % 11;  // This will generate a value between 0 and 10 inclusive
        int delta_i;
        if (r >= 0 && r < 7) {
            // Generate Δi to be -1, 0, or +1
            delta_i = (rand() % 3) - 1; // Generates -1, 0, or +1
            // printf("Generated Δi: %d\n", delta_i);
        } else {
            // Generate the new page reference “j,” 2 ≤ |Δi| ≤ page_size - 1
            delta_i = (rand() % (page_size - 1)) + 2; // Generates values between - page_size - 1 and page_size - 1
            int sign = rand() % 2;
            delta_i = sign == 0? -delta_i : delta_i;
        }
    return delta_i;
}

int get_free_page_size() {
    FreePageList* current = free_page_list;
    int i = 0;
    while(current != NULL) {
        current = current->next;
        i++;
    }
    return i;
}

void initialize_page_table(Process* proc) {
    for(int i = 0; i < proc->size_pages; i++) {
        proc->page_table[i] = -1;
    }
}

int main(int argc, char* argv[]) {
    int stepTwo = 0;

    // Process sizes (in pages/MB)
    int process_sizes[4] = {5, 11, 17, 31};
    char* current_algorithm_name;

    if (argc < 3){
        printf("usage: %s [FIFO, LRU, LFU, MFU or Random] randomSeed(RAND for random).\n", argv[0]);
        return -1;
    }

    if(strcmp(argv[1], "FIFO") == 0){
        current_algorithm = FIFO;
        current_algorithm_name = "FIFO";
    }else if(strcmp(argv[1], "LRU") == 0){
        current_algorithm = LRU;
        current_algorithm_name = "LRU";
    }else if(strcmp(argv[1], "LFU") == 0){
        current_algorithm = LFU;
        current_algorithm_name = "LFU";
    }else if(strcmp(argv[1], "MFU") == 0){
        current_algorithm = MFU;
        current_algorithm_name = "MFU";
    }else if(strcmp(argv[1], "Random") == 0){
        current_algorithm = RANDOM_REPL;
        current_algorithm_name = "RANDOM_REPL";
    }else {
        printf("usage: %s [FIFO, LRU, LFU, MFU or Random] randomSeed(RAND for random).\n", argv[0]);
        return -1;
    }


    if (argc == 4 && atoi(argv[3]) == 1) {     
        stepTwo = 1;
    }

    // Seed for random number generation
    if (strcmp(argv[2], "RAND") == 0)
        srand(time(NULL));  // Use system time if 'RAND' is specified
    else
        srand(atoi(argv[2]));  // Use provided seed value

    // Array of processes
    Process Q[TOTAL_PROCESSES];

    // char* current_algorithm_name = "RANDOM_REPL";
    // current_algorithm = RANDOM_REPL;

    // Initialize 150 processes
    for (int i = 0; i < TOTAL_PROCESSES; i++) {
        
        // Q[i].name = "P" + i;  // Assign process ID
        snprintf(Q[i].name, sizeof(Q[i].name), "P%d", i);   // to set the name as P0, P1, P2... etc
        Q[i].size_mb = process_sizes[rand() % 4];  // Random process size from options
        Q[i].size_pages = Q[i].size_mb; // --> extra value used
        Q[i].arrival_time = rand() % 60;  // Random arrival time (within 60 seconds)
        Q[i].service_duration = rand() % (MAX_SERVICE_DURATION + 1);  // Random service duration (1-5 seconds)
        if(Q[i].service_duration < MIN_SERVICE_DURATION) {
            Q[i].service_duration = MIN_SERVICE_DURATION;
        }
        Q[i].remaining_time = Q[i].service_duration * 1000;    // initially set to service time
        Q[i].page_table = malloc(sizeof(int) * Q[i].size_pages);     //initally set to null
        Q[i].current_page = 0;  // All processes start with page 0
        initialize_page_table(&Q[i]);
    }

    // Sort processes based on their arrival time
    qsort(Q, TOTAL_PROCESSES, sizeof(Process), cmp_arrival_time);

    // Print process information
    /*for (int i = 0; i < TOTAL_PROCESSES; i++) {
        printf("Process ID: %s, Size: %d MB, Arrival Time: %d seconds, Duration: %d seconds\n",
               Q[i].name, Q[i].size_mb, Q[i].arrival_time, Q[i].service_duration);
    }*/
 
    // initialise free page list and the memory map
    initialize_free_page_list();
    initialize_memory_map();
    create_outputs_directory();

    char page_replacement_filename[256];
    snprintf(page_replacement_filename, sizeof(page_replacement_filename), "outputs/%s-run.txt", current_algorithm_name);
    FILE* fp = fopen(page_replacement_filename, "w");

    Statistics* stats = malloc(sizeof(Statistics));
    
    int end = -1;
    for (int sim_time = 0; sim_time < SIMULATION_TIME_SEC * 1000; sim_time += 100) {

        for (int i = end + 1; i < TOTAL_PROCESSES; i++) {
            if ((Q[i].arrival_time * 1000 <= sim_time) && (get_free_page_size() >= 4)) {
                fprintf(fp, "[Time %d.%03d] Process: %s, Enter, Size: %d pages, Service Duration: %d seconds\n", 
                        sim_time / 1000, sim_time % 1000, 
                        Q[i].name, Q[i].size_pages, Q[i].service_duration);
                handle_page_fault(&Q[i], Q[i].current_page, sim_time, stats, fp, Q);
                print_memory_map(fp);
                end = i;
                stats->miss_count++;
            } else break;
        }

        for(int i = 0; i <= end; i++) {
            
            // if process has not finished
            if(Q[i].remaining_time > 0) {
                // update current page to random reference page
                Q[i].current_page = Q[i].current_page + generate_delta_i(Q[i].size_pages);

                if(Q[i].current_page < 0) 
                    Q[i].current_page += Q[i].size_pages;
                else    
                    Q[i].current_page = Q[i].current_page % Q[i].size_pages;
                
                
                // check if page is in memory
                stats->total_references++;
                int physical_page = -1;
                if(is_page_in_memory(&Q[i], Q[i].current_page, &physical_page)) {
                    if(physical_page == -1) {
                        fprintf(fp, "Page not in memory!\n");
                        continue;
                    }
                    //fprintf(fp, "Page %d for process %s found in memory\n", Q[i].current_page, Q[i].name);
                    fprintf(fp, "[Time %d.%03d] Process: %s, Page-Referenced: %d, Page-In-Memory: %d \n", 
                        sim_time / 1000, sim_time % 1000,
                        Q[i].name, Q[i].current_page, physical_page);
                    // update last access time and count
                    memory_map.pages[physical_page]->last_access_time = sim_time;
                    memory_map.pages[physical_page]->access_count += 1;
                    
                    //update hit count
                    stats->hit_count++;
                } else {
                    handle_page_fault(&Q[i], Q[i].current_page, sim_time, stats, fp, Q);
                    //update miss count
                    stats->miss_count++;
                }
                Q[i].remaining_time -= 100;
                if(Q[i].remaining_time <= 0) {
                    release_pages(&Q[i]);
                    fprintf(fp, "[Time %d.%03d] Process: %s, Exit, Size: %d pages, Service Duration: %d seconds\n", 
                        sim_time / 1000, sim_time % 1000,
                        Q[i].name, Q[i].size_pages, Q[i].service_duration);
                    print_memory_map(fp);
                }
            }
        }
        usleep(1000);
    }
    printf("Number of processes that were sucessfully swapped is %d\n", (stats->processes_swapped_in));
    printf("Hit/Miss - %f\n", (float)(stats->hit_count)/(float)(stats->miss_count));
    printf("Hits - %d\n", (stats->hit_count));
    printf("Miss - %d\n", (stats->miss_count));
    printf("Total Page References - %d\n", (stats->total_references));
    return 0;
}
