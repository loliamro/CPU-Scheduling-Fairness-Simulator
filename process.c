#include <stdio.h>
#include "process.h"

// Print one process's details (useful for debugging)
void printProcess(Process p) {
    char *typeName;
    if      (p.type == INTERACTIVE) typeName = "Interactive";
    else if (p.type == BATCH)       typeName = "Batch";
    else                            typeName = "Real-Time";

    printf("PID: %d | Type: %-12s | Arrival: %d | Burst: %d | Priority: %d\n",
        p.pid, typeName, p.arrival_time, p.burst_time, p.priority);
}