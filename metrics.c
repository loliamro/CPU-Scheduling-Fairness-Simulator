#include <stdio.h>
#include <string.h>
#include <math.h>
#include "metrics.h"

// ─────────────────────────────────────────────
// Jain's Fairness Index
// Measures how equally CPU time is distributed
// Result: 0 = completely unfair, 1 = perfectly fair
// Formula: (sum of x)^2 / (n * sum of x^2)
// ─────────────────────────────────────────────
float jainsIndex(Process processes[], int count) {
    float sum    = 0.0;
    float sumSq  = 0.0;

    for (int i = 0; i < count; i++) {
        // We measure fairness based on turnaround time
        float x = (float)processes[i].turnaround_time;
        sum   += x;
        sumSq += x * x;
    }

    if (sumSq == 0) return 1.0;  // avoid division by zero

    return (sum * sum) / ((float)count * sumSq);
}

// ─────────────────────────────────────────────
// Calculate all metrics after an algorithm runs
// ─────────────────────────────────────────────
Metrics calculateMetrics(Process processes[], int count,
                         char *algorithmName, int totalTime) {
    Metrics m;
    strncpy(m.algorithmName, algorithmName, sizeof(m.algorithmName));

    m.totalProcesses = count;
    m.totalTime      = totalTime;

    // Sum up all times
    float totalWaiting    = 0;
    float totalTurnaround = 0;
    float totalResponse   = 0;
    float totalBurst      = 0;

    for (int i = 0; i < count; i++) {
        totalWaiting    += processes[i].waiting_time;
        totalTurnaround += processes[i].turnaround_time;
        totalResponse   += processes[i].response_time;
        totalBurst      += processes[i].burst_time;
    }

    // Calculate averages
    m.avgWaitingTime    = totalWaiting    / count;
    m.avgTurnaroundTime = totalTurnaround / count;
    m.avgResponseTime   = totalResponse   / count;

    // CPU Utilization = total burst time / total simulation time
    m.cpuUtilization = (totalBurst / (float)totalTime) * 100.0;

    // Throughput = processes completed per unit time
    m.throughput = (float)count / (float)totalTime;

    // Jain's Fairness Index
    m.fairnessIndex = jainsIndex(processes, count);

    return m;
}

// ─────────────────────────────────────────────
// Print metrics to terminal in a nice table
// ─────────────────────────────────────────────
void printMetrics(Metrics m) {
    printf("\n");
    printf("╔══════════════════════════════════════════════╗\n");
    printf("║  RESULTS: %-35s║\n", m.algorithmName);
    printf("╠══════════════════════════════════════════════╣\n");
    printf("║  Total Processes      : %-20d║\n", m.totalProcesses);
    printf("║  Total Time           : %-20d║\n", m.totalTime);
    printf("╠══════════════════════════════════════════════╣\n");
    printf("║  Avg Waiting Time     : %-20.2f║\n", m.avgWaitingTime);
    printf("║  Avg Turnaround Time  : %-20.2f║\n", m.avgTurnaroundTime);
    printf("║  Avg Response Time    : %-20.2f║\n", m.avgResponseTime);
    printf("╠══════════════════════════════════════════════╣\n");
    printf("║  CPU Utilization      : %-19.2f%%║\n", m.cpuUtilization);
    printf("║  Throughput           : %-20.4f║\n", m.throughput);
    printf("║  Fairness Index       : %-20.4f║\n", m.fairnessIndex);
    printf("╚══════════════════════════════════════════════╝\n");
}

// ─────────────────────────────────────────────
// Save metrics to a file (for Person 3's graphs)
// ─────────────────────────────────────────────
void saveMetricsToFile(Metrics m, FILE *file) {
    fprintf(file, "%s,%.2f,%.2f,%.2f,%.2f,%.4f,%.4f\n",
        m.algorithmName,
        m.avgWaitingTime,
        m.avgTurnaroundTime,
        m.avgResponseTime,
        m.cpuUtilization,
        m.throughput,
        m.fairnessIndex);
}
