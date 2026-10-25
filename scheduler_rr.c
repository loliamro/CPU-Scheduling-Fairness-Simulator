#include <stdio.h>
#include <string.h>
#include "scheduler_rr.h"

// ─────────────────────────────────────────────
// ROUND ROBIN SCHEDULER
// Each process gets a fixed time slice (quantum)
// If not finished, it goes back to end of queue
// Very fair — everyone gets equal turns
// ─────────────────────────────────────────────
Metrics roundRobin(Process processes[], int count, int quantum, int scenario) {

    // Step 1: Make a local copy so we don't modify the original
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

    // Step 2: Ready queue — stores index of processes ready to run
    int queue[500];
    int front    = 0, rear = 0;
    int time     = 0;   // current clock time
    int completed = 0;  // how many processes finished
    int inQueue[50] = {0};

    // Step 3: Add processes that arrive at time 0
    for (int i = 0; i < count; i++) {
        if (local[i].arrival_time == 0) {
            queue[rear++] = i;
            inQueue[i]    = 1;
        }
    }

    // Step 4: Main scheduling loop
    while (completed < count) {

        // If queue is empty, advance time to next arrival
        if (front == rear) {
            int nextArrival = 99999;
            for (int i = 0; i < count; i++) {
                if (local[i].remaining_time > 0 &&
                    local[i].arrival_time > time &&
                    local[i].arrival_time < nextArrival) {
                    nextArrival = local[i].arrival_time;
                }
            }
            time = nextArrival;

            // Add newly arrived processes
            for (int i = 0; i < count; i++) {
                if (local[i].arrival_time <= time &&
                    local[i].remaining_time > 0 &&
                    !inQueue[i]) {
                    queue[rear++] = i;
                    inQueue[i]    = 1;
                }
            }
            continue;
        }

        // Get next process from front of queue
        int idx      = queue[front++];
        inQueue[idx] = 0;

        // Record start time (first time process gets CPU)
        if (local[idx].start_time == -1) {
            local[idx].start_time = time;
        }

        // Run for quantum or remaining time (whichever is less)
        int runTime = (local[idx].remaining_time < quantum)
                    ? local[idx].remaining_time
                    : quantum;

        // Advance clock
        time                      += runTime;
        local[idx].remaining_time -= runTime;

        // Add any newly arrived processes to queue
        for (int i = 0; i < count; i++) {
            if (local[i].arrival_time <= time  &&
                local[i].remaining_time > 0    &&
                !inQueue[i] && i != idx) {
                queue[rear++] = i;
                inQueue[i]    = 1;
            }
        }

        // If process is not done, put it back in queue
        if (local[idx].remaining_time > 0) {
            queue[rear++] = idx;
            inQueue[idx]  = 1;
        }
        // If process is done, record finish time
        else {
            local[idx].finish_time     = time;
            local[idx].turnaround_time = time - local[idx].arrival_time;
            local[idx].waiting_time    = local[idx].turnaround_time
                                       - local[idx].burst_time;
            local[idx].response_time   = local[idx].start_time
                                       - local[idx].arrival_time;
            completed++;
        }
    }

    // Step 5: Calculate and return metrics
    char algoName[50];
    snprintf(algoName, sizeof(algoName), "Round Robin (Q=%d)", quantum);
    return calculateMetrics(local, count, algoName, time);
}