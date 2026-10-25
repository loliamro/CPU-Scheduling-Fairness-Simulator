#ifndef WORKLOAD_H
#define WORKLOAD_H

#include "process.h"

// Maximum number of processes in a scenario
#define MAX_PROCESSES 50

// Scenario types
#define SCENARIO_A  0   // Mixed: Interactive + Batch + Real-Time
#define SCENARIO_B  1   // Batch only
#define SCENARIO_C  2   // Real-Time + Interactive heavy
#define SCENARIO_D  3   // Balanced equal thirds

// Function declarations
int generateWorkload(Process processes[], int scenario);
void printWorkload(Process processes[], int count);

#endif