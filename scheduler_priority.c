#include <stdio.h>
#include <string.h>
#include "scheduler_priority.h"

// ─────────────────────────────────────────────
// HELPER: Copy processes into a fresh local array
// ─────────────────────────────────────────────
static void copyProcesses(Process local[], Process processes[], int count) {
    for (int i = 0; i < count; i++) {
        local[i]                 = processes[i];
        local[i].remaining_time  = processes[i].burst_time;
        local[i].start_time      = -1;
        local[i].finish_time     = -1;
        local[i].waiting_time    = 0;
        local[i].turnaround_time = 0;
        local[i].response_time   = 0;
    }
}

// ─────────────────────────────────────────────
// PRIORITY SCHEDULING (Non-Preemptive)
// CPU picks the highest priority process
// from all arrived processes.
// Once running, process cannot be interrupted.
// Problem: LOW priority processes may STARVE
// ─────────────────────────────────────────────
Metrics priorityScheduling(Process processes[], int count, int scenario) {

    Process local[50];
    copyProcesses(local, processes, count);

    int time      = 0;
    int completed = 0;
    int done[50]  = {0};  // track which processes are finished

    while (completed < count) {

        // Find highest priority process that has arrived
        // (lowest priority NUMBER = highest priority)
        int selected = -1;
        for (int i = 0; i < count; i++) {
            if (!done[i] && local[i].arrival_time <= time) {
                if (selected == -1 ||
                    local[i].priority < local[selected].priority) {
                    selected = i;
                }
            }
        }

        // If no process available, advance time to next arrival
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

        // Run selected process to completion (non-preemptive)
        local[selected].start_time  = time;
        time                       += local[selected].burst_time;
        local[selected].finish_time = time;

        // Calculate metrics for this process
        local[selected].turnaround_time = local[selected].finish_time
                                        - local[selected].arrival_time;
        local[selected].waiting_time    = local[selected].turnaround_time
                                        - local[selected].burst_time;
        local[selected].response_time   = local[selected].start_time
                                        - local[selected].arrival_time;

        done[selected] = 1;
        completed++;
    }

    return calculateMetrics(local, count, "Priority (No Aging)", time);
}

// ─────────────────────────────────────────────
// PRIORITY SCHEDULING WITH AGING
// Same as above BUT waiting processes get their
// priority boosted every 10 time units.
// This prevents starvation of low priority jobs.
// ─────────────────────────────────────────────
Metrics priorityWithAging(Process processes[], int count, int scenario) {

    Process local[50];
    copyProcesses(local, processes, count);

    // Track how long each process has been waiting
    int waitedFor[50] = {0};
    // Track current (dynamic) priority — starts at original
    int dynPriority[50];
    for (int i = 0; i < count; i++) {
        dynPriority[i] = local[i].priority;
    }

    int time      = 0;
    int completed = 0;
    int done[50]  = {0};

    while (completed < count) {

        // ── Aging: boost priority of waiting processes ──
        // Every 10 time units, reduce priority number by 1
        // (lower number = higher priority)
        for (int i = 0; i < count; i++) {
            if (!done[i] && local[i].arrival_time <= time) {
                waitedFor[i]++;
                // Boost every 10 time units, minimum priority = 1
                if (waitedFor[i] % 10 == 0 && dynPriority[i] > 1) {
                    dynPriority[i]--;
                }
            }
        }

        // Find highest dynamic priority process that has arrived
        int selected = -1;
        for (int i = 0; i < count; i++) {
            if (!done[i] && local[i].arrival_time <= time) {
                if (selected == -1 ||
                    dynPriority[i] < dynPriority[selected]) {
                    selected = i;
                }
            }
        }

        // If no process available, advance time
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
        local[selected].start_time  = time;
        time                       += local[selected].burst_time;
        local[selected].finish_time = time;

        // Calculate metrics
        local[selected].turnaround_time = local[selected].finish_time
                                        - local[selected].arrival_time;
        local[selected].waiting_time    = local[selected].turnaround_time
                                        - local[selected].burst_time;
        local[selected].response_time   = local[selected].start_time
                                        - local[selected].arrival_time;

        done[selected] = 1;
        completed++;
        waitedFor[selected] = 0;  // reset wait counter
    }

    return calculateMetrics(local, count,
                            "Priority (With Aging)", time);
}