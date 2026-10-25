#ifndef METRICS_H
#define METRICS_H

#include "process.h"

// Struct to hold all results for one algorithm run
typedef struct {
    char algorithmName[50];  // e.g., "Round Robin (Q=4)"

    // Averages across all processes
    float avgWaitingTime;
    float avgTurnaroundTime;
    float avgResponseTime;

    // Overall system metrics
    float cpuUtilization;    // % of time CPU was busy
    float throughput;        // processes completed per unit time
    float fairnessIndex;     // Jain's Fairness Index (0 to 1)

    // Raw totals (used for calculations)
    int totalProcesses;
    int totalTime;           // total simulation time

} Metrics;

// Function declarations
Metrics calculateMetrics(Process processes[], int count, 
                         char *algorithmName, int totalTime);
void printMetrics(Metrics m);
void saveMetricsToFile(Metrics m, FILE *file);
float jainsIndex(Process processes[], int count);

#endif