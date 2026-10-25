#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "workload.h"

// ─────────────────────────────────────────────
// HELPER: Create one Interactive process
// Short burst, frequent arrivals, medium priority
// Example: keyboard input, terminal commands
// ─────────────────────────────────────────────
static Process createInteractive(int pid, int arrival) {
    Process p;
    p.pid            = pid;
    p.type           = INTERACTIVE;
    p.arrival_time   = arrival;
    p.burst_time     = (rand() % 5) + 1;   // 1 to 5ms
    p.priority       = (rand() % 3) + 3;   // 3 to 5 (medium)
    p.remaining_time = p.burst_time;
    p.start_time     = -1;
    p.finish_time    = -1;
    p.waiting_time   = 0;
    p.turnaround_time = 0;
    p.response_time  = 0;
    snprintf(p.name, sizeof(p.name), "P%d(INT)", pid);
    return p;
}

// ─────────────────────────────────────────────
// HELPER: Create one Batch process
// Long burst, spread out arrivals, low priority
// Example: file compression, video rendering
// ─────────────────────────────────────────────
static Process createBatch(int pid, int arrival) {
    Process p;
    p.pid            = pid;
    p.type           = BATCH;
    p.arrival_time   = arrival;
    p.burst_time     = (rand() % 31) + 20;  // 20 to 50ms
    p.priority       = (rand() % 5) + 6;    // 6 to 10 (low)
    p.remaining_time = p.burst_time;
    p.start_time     = -1;
    p.finish_time    = -1;
    p.waiting_time   = 0;
    p.turnaround_time = 0;
    p.response_time  = 0;
    snprintf(p.name, sizeof(p.name), "P%d(BAT)", pid);
    return p;
}

// ─────────────────────────────────────────────
// HELPER: Create one Real-Time process
// Medium burst, highest priority, strict deadlines
// Example: audio processing, sensor reading
// ─────────────────────────────────────────────
static Process createRealTime(int pid, int arrival) {
    Process p;
    p.pid            = pid;
    p.type           = REAL_TIME;
    p.arrival_time   = arrival;
    p.burst_time     = (rand() % 8) + 3;   // 3 to 10ms
    p.priority       = (rand() % 2) + 1;   // 1 to 2 (highest)
    p.remaining_time = p.burst_time;
    p.start_time     = -1;
    p.finish_time    = -1;
    p.waiting_time   = 0;
    p.turnaround_time = 0;
    p.response_time  = 0;
    snprintf(p.name, sizeof(p.name), "P%d(RT)", pid);
    return p;
}

// ─────────────────────────────────────────────
// SCENARIO A: Typical Mixed Workload
// 10 Interactive + 5 Batch + 3 Real-Time = 18 processes
// ─────────────────────────────────────────────
static int scenarioA(Process processes[]) {
    int count = 0;
    int pid   = 1;

    // 10 Interactive — arrive frequently every 1-3 units
    int arrival = 0;
    for (int i = 0; i < 10; i++) {
        processes[count++] = createInteractive(pid++, arrival);
        arrival += (rand() % 3) + 1;  // next arrives 1-3 units later
    }

    // 5 Batch — arrive spread out every 10-20 units
    arrival = 5;
    for (int i = 0; i < 5; i++) {
        processes[count++] = createBatch(pid++, arrival);
        arrival += (rand() % 11) + 10;
    }

    // 3 Real-Time — arrive unpredictably
    arrival = 2;
    for (int i = 0; i < 3; i++) {
        processes[count++] = createRealTime(pid++, arrival);
        arrival += (rand() % 15) + 5;
    }

    return count;
}

// ─────────────────────────────────────────────
// SCENARIO B: Batch Only
// 15 Batch processes — tests fairness on long jobs
// ─────────────────────────────────────────────
static int scenarioB(Process processes[]) {
    int count   = 0;
    int pid     = 1;
    int arrival = 0;

    for (int i = 0; i < 15; i++) {
        processes[count++] = createBatch(pid++, arrival);
        arrival += (rand() % 11) + 10;
    }

    return count;
}

// ─────────────────────────────────────────────
// SCENARIO C: Real-Time + Interactive Heavy
// 5 Real-Time + 10 Interactive = 15 processes
// Tests how algorithms handle high-priority loads
// ─────────────────────────────────────────────
static int scenarioC(Process processes[]) {
    int count = 0;
    int pid   = 1;

    // 5 Real-Time
    int arrival = 0;
    for (int i = 0; i < 5; i++) {
        processes[count++] = createRealTime(pid++, arrival);
        arrival += (rand() % 5) + 2;
    }

    // 10 Interactive
    arrival = 1;
    for (int i = 0; i < 10; i++) {
        processes[count++] = createInteractive(pid++, arrival);
        arrival += (rand() % 3) + 1;
    }

    return count;
}

// ─────────────────────────────────────────────
// SCENARIO D: Balanced Equal Thirds
// 7 Interactive + 7 Batch + 6 Real-Time = 20 processes
// ─────────────────────────────────────────────
static int scenarioD(Process processes[]) {
    int count = 0;
    int pid   = 1;

    // 7 Interactive
    int arrival = 0;
    for (int i = 0; i < 7; i++) {
        processes[count++] = createInteractive(pid++, arrival);
        arrival += (rand() % 3) + 1;
    }

    // 7 Batch
    arrival = 3;
    for (int i = 0; i < 7; i++) {
        processes[count++] = createBatch(pid++, arrival);
        arrival += (rand() % 11) + 10;
    }

    // 6 Real-Time
    arrival = 1;
    for (int i = 0; i < 6; i++) {
        processes[count++] = createRealTime(pid++, arrival);
        arrival += (rand() % 8) + 3;
    }

    return count;
}

// ─────────────────────────────────────────────
// MAIN: Generate workload based on scenario
// ─────────────────────────────────────────────
int generateWorkload(Process processes[], int scenario) {
    srand(42);  // Fixed seed = same results every run (important for fairness)

    switch (scenario) {
        case SCENARIO_A: return scenarioA(processes);
        case SCENARIO_B: return scenarioB(processes);
        case SCENARIO_C: return scenarioC(processes);
        case SCENARIO_D: return scenarioD(processes);
        default:
            printf("Unknown scenario!\n");
            return 0;
    }
}

// ─────────────────────────────────────────────
// PRINT: Display all processes in a workload
// ─────────────────────────────────────────────
void printWorkload(Process processes[], int count) {
    printf("\n%-6s %-12s %-10s %-8s %-10s %-10s\n",
        "PID", "Name", "Type", "Arrival", "Burst", "Priority");
    printf("─────────────────────────────────────────────────────────\n");

    for (int i = 0; i < count; i++) {
        char *typeName;
        if      (processes[i].type == INTERACTIVE) typeName = "Interactive";
        else if (processes[i].type == BATCH)       typeName = "Batch";
        else                                       typeName = "Real-Time";

        printf("%-6d %-12s %-10s %-8d %-10d %-10d\n",
            processes[i].pid,
            processes[i].name,
            typeName,
            processes[i].arrival_time,
            processes[i].burst_time,
            processes[i].priority);
    }
    printf("─────────────────────────────────────────────────────────\n");
    printf("Total Processes: %d\n\n", count);
}