#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

// Placeholder for process definition and Total_PROCESS constant
#define Total_PROCESS 150
#define PROCss_DuraTN 5 // Maximum process duration

typedef struct {
    int pid;          // Process ID
    int PGECNTER;     // Number of pages (process size in MB)
    int TIMEARR;      // Arrival time (in seconds)
    int DRUTION;      // Service duration (in seconds)
    int PGCRR;        // Current page (starting at 0)
} process;

// Function to compare arrival times for sorting
int CMP_ARRtime(const void* a, const void* b) {
    return ((process*)a)->TIMEARR - ((process*)b)->TIMEARR;
}

int main(int argc, char* argv[]) {
    int stepTwo = 0;

    // Process sizes (in pages/MB)
    int PGCoptn[4] = {5, 11, 17, 31};

    if (argc == 4 && atoi(argv[3]) == 1) {
        stepTwo = 1;
    }

    // Seed for random number generation
    if (strcmp(argv[2], "RAND") == 0)
        srand(time(NULL));  // Use system time if 'RAND' is specified
    else
        srand(atoi(argv[2]));  // Use provided seed value

    // Array of processes
    process Q[Total_PROCESS];

    // Initialize 150 processes
    for (int i = 0; i < Total_PROCESS; i++) {
        Q[i].pid = i;  // Assign process ID
        Q[i].PGECNTER = PGCoptn[rand() % 4];  // Random process size from options
        Q[i].TIMEARR = rand() % 60;  // Random arrival time (within 60 seconds)
        Q[i].DRUTION = rand() % PROCss_DuraTN + 1;  // Random service duration (1-5 seconds)
        Q[i].PGCRR = 0;  // All processes start with page 0
    }

    // Sort processes based on their arrival time
    qsort(Q, Total_PROCESS, sizeof(process), CMP_ARRtime);

    // Print process information
    for (int i = 0; i < Total_PROCESS; i++) {
        printf("Process ID: %d, Size: %d MB, Arrival Time: %d seconds, Duration: %d seconds\n",
               Q[i].pid, Q[i].PGECNTER, Q[i].TIMEARR, Q[i].DRUTION);
    }

    return 0;
}
