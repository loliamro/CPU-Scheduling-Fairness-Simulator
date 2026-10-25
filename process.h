#ifndef PROCESS_H
#define PROCESS_H

// Process types
#define INTERACTIVE  0
#define BATCH        1
#define REAL_TIME    2

// Process struct — represents one process in the simulator
typedef struct {

    // --- Identity ---
    int pid;              // Process ID (1, 2, 3, ...)
    char name[20];        // Process name (e.g., "P1", "P2")
    int type;             // Type: INTERACTIVE, BATCH, or REAL_TIME

    // --- Input (given before scheduling) ---
    int arrival_time;     // When process enters the ready queue
    int burst_time;       // Total CPU time needed
    int priority;         // Priority level (1 = highest, 10 = lowest)

    // --- Tracking (updated during scheduling) ---
    int remaining_time;   // How much burst time is left (used by RR)
    int start_time;       // When process first got the CPU
    int finish_time;      // When process completely finished

    // --- Results (calculated after scheduling) ---
    int waiting_time;     // Time spent waiting in queue
    int turnaround_time;  // Total time from arrival to finish
    int response_time;    // Time from arrival to first CPU access

} Process;

// Function declarations
void printProcess(Process p);

#endif