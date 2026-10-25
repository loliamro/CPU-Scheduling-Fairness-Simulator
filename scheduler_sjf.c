#include <stdio.h>
#include <string.h>
#include "process.h"
#include "metrics.h"
#include "scheduler_sjf.h"

// ─────────────────────────────────────────────
// SJF - Shortest Job First (Non-Preemptive)
// Rule: pick the shortest burst from arrived processes
// Non-preemptive: runs to completion once selected
// Problem: long jobs may starve if short jobs keep arriving
// ─────────────────────────────────────────────
Metrics sjf(Process processes[], int count, int scenario) {

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

    int done[50]  = {0};  // track finished processes
    int time      = 0;
    int completed = 0;

    // Step 2: Main loop
    while (completed < count) {

        // Find shortest burst among arrived processes
        int selected  = -1;
        int minBurst  = 99999;

        for (int i = 0; i < count; i++) {
            if (!done[i] && local[i].arrival_time <= time) {
                if (local[i].burst_time < minBurst) {
                    minBurst = local[i].burst_time;
                    selected = i;
                }
            }
        }

        // If no process arrived yet, jump to next arrival
        if (selected == -1) {
            int nextArrival = 99999;
            for (int i = 0; i < count; i++) {
                if (!done[i] && local[i].arrival_time < nextArrival) {
                    nextArrival = local[i].arrival_time;
                }
            }
            time = nextArrival;
            continue;
        }

        // Run selected process to completion
        local[selected].start_time   = time;
        time                        += local[selected].burst_time;
        local[selected].finish_time  = time;

        // Calculate metrics
        local[selected].turnaround_time = local[selected].finish_time
                                        - local[selected].arrival_time;
        local[selected].waiting_time    = local[selected].turnaround_time
                                        - local[selected].burst_time;
        local[selected].response_time   = local[selected].start_time
                                        - local[selected].arrival_time;

        done[selected] = 1;
        completed++;
    }

    return calculateMetrics(local, count, "SJF", time);
}