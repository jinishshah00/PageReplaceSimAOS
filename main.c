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
int CMP_ARRtime(const void* a, const void* b) {
    return ((Process*)a)->arrival_time - ((Process*)b)->arrival_time;
}

int main(int argc, char* argv[]) {
    int stepTwo = 0;

    // Process sizes (in pages/MB)
    int process_sizes[4] = {5, 11, 17, 31};

    if (argc == 4 && atoi(argv[3]) == 1) {      // ---> why?
        stepTwo = 1;
    }

    // Seed for random number generation
    if (strcmp(argv[2], "RAND") == 0)
        srand(time(NULL));  // Use system time if 'RAND' is specified
    else
        srand(atoi(argv[2]));  // Use provided seed value

    // Array of processes
    Process Q[TOTAL_PROCESSES];

    // Initialize 150 processes
    for (int i = 0; i < TOTAL_PROCESSES; i++) {
        
        // Q[i].name = "P" + i;  // Assign process ID
        snprintf(Q[i].name, 4, "P%d", i);   // to set the name as P0, P1, P2... etc
        Q[i].size_mb = process_sizes[rand() % 4];  // Random process size from options
        Q[i].size_pages = Q[i].size_mb; // --> extra value used
        Q[i].arrival_time = rand() % 60;  // Random arrival time (within 60 seconds)
        Q[i].service_duration = Max(rand() % MAX_SERVICE_DURATION, MIN_SERVICE_DURATION);  // Random service duration (1-5 seconds)
        Q[i].remaining_time = Q[i].service_duration;    // initially set to service time
        Q[i].page_table = NULL;     //initally set to null
        Q[i].current_page = 0;  // All processes start with page 0
    }

    // Sort processes based on their arrival time
    qsort(Q, TOTAL_PROCESSES, sizeof(Process), CMP_ARRtime);

    // Print process information
    for (int i = 0; i < TOTAL_PROCESSES; i++) {
        printf("Process ID: %s, Size: %d MB, Arrival Time: %d seconds, Duration: %d seconds\n",
               Q[i].name, Q[i].size_mb, Q[i].arrival_time, Q[i].service_duration);
    }


    // initialise free page list and the memory map
    initialize_free_page_list();
    initialize_memory_map();

    int generate_delta_i(){
        r = rand() % 11;  // This will generate a value between 0 and 10 inclusive
        printf("Random r value: %d\n", r);
        int delta_i;
        if (r >= 0 && r < 7) {
            // Generate Δi to be -1, 0, or +1
            delta_i = (rand() % 3) - 1; // Generates -1, 0, or +1
            // printf("Generated Δi: %d\n", delta_i);
        } else if (r >= 7 && r <= 10) {
            // Generate the new page reference “j,” 2 ≤ |Δi| ≤ 9            
            do {
                delta_i = (rand() % 18) - 9; // Generates values between -9 and 9
            } while (delta_i == 0 || abs(delta_i) < 2); // Ensure |Δi| is between 2 and 9

        
        }
        return delta_i;
    }

    
    Statistics* stats;
    FILE* fp;
    

    for (float sim_time = 0.0; sim_time < SIMULATION_TIME_SEC; sim_time + 0.1){

        for (int i=0; i < TOTAL_PROCESSES; i++){
            job_queue.head = Q[i];
            if (Q[i].arrival_time <= sim_time){
                
                if (is_page_in_memory(job_queue.head, job_queue.head.current_page, job_queue.head.page_table)){
                    /// Need to update statistics hit count

                }

                else if (free_page_list != NULL){
                    Q[i].page_table[Q[i].current_page] = free_page_list->page_number;
                    free_page_list = free_page_list->next
                }

                else{
                    swap_page_in(job_queue.head, job_queue.head.current_page, sim_time, stats,  fp)
                }
                    

                int delta_i = generate_delta_i();
                int next_page = Q[i].current_page + delta_i;
            }
        }
    }

    return 0;
}
