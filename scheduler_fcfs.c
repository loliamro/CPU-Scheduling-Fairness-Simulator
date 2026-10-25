#include <stdio.h>
#include <string.h>
#include "process.h"
#include "metrics.h"
#include "scheduler_fcfs.h"

// ─────────────────────────────────────────────
// FCFS - First Come First Served
// Rule: whoever arrives first runs first
// Non-preemptive: runs until completion
// Problem: long jobs block short ones (convoy effect)
// ─────────────────────────────────────────────
Metrics fcfs(Process processes[], int count, int scenario) {

    // Step 1: Make a local copy
    Process local[50];
    for (int i = 0; i < count; i++) {
        local[i]                 = processes[i];
        local[i].remaining_time  = processes[i].burst_time;
        local[i].start_time      = -1;
        local[i].finish_time     = -1;
        local[i].waiting_time    = 0;
        local[i].turnaround_time = 0;
        local[i].response_time   = 0;
    }

    // Step 2: Sort by arrival time (bubble sort)
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (local[j].arrival_time > local[j + 1].arrival_time) {
                Process temp  = local[j];
                local[j]      = local[j + 1];
                local[j + 1]  = temp;
            }
        }
    }

    // Step 3: Run each process in arrival order
    int time = 0;
    for (int i = 0; i < count; i++) {

        // If CPU is idle, jump to next arrival
        if (time < local[i].arrival_time) {
            time = local[i].arrival_time;
        }

        // Record start time
        local[i].start_time  = time;

        // Run to completion
        time                += local[i].burst_time;
        local[i].finish_time = time;

        // Calculate metrics
        local[i].turnaround_time = local[i].finish_time
                                 - local[i].arrival_time;
        local[i].waiting_time    = local[i].turnaround_time
                                 - local[i].burst_time;
        local[i].response_time   = local[i].start_time
                                 - local[i].arrival_time;
    }

    return calculateMetrics(local, count, "FCFS", time);
}