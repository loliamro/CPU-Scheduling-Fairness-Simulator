#include <stdio.h>
#include <stdlib.h>
#include "process.h"
#include "workload.h"
#include "metrics.h"
#include "scheduler_rr.h"
#include "scheduler_priority.h"
#include "scheduler_fcfs.h"
#include "scheduler_sjf.h"
#include "scheduler_advanced1.h"   /* CFS              */
#include "scheduler_advanced2.h"   /* Lottery          */

/* ── Print table header ──────────────────────────────────────────────── */
void printTableHeader() {
    printf("\n──────────────────────────────────────────────────────────────────────────\n");
    printf("%-25s %-12s %-14s %-14s %-12s %-10s\n",
           "Algorithm", "Avg Wait", "Avg Turnaround",
           "Avg Response", "CPU Util%", "Fairness");
    printf("──────────────────────────────────────────────────────────────────────────\n");
}

/* ── Print one result row ────────────────────────────────────────────── */
void printRow(Metrics m) {
    printf("%-25s %-12.2f %-14.2f %-14.2f %-12.2f %-10.4f\n",
           m.algorithmName,
           m.avgWaitingTime,
           m.avgTurnaroundTime,
           m.avgResponseTime,
           m.cpuUtilization,
           m.fairnessIndex);
}

/* ── Save one row to CSV ─────────────────────────────────────────────── */
void saveRow(FILE *file, Metrics m, char *scenario) {
    fprintf(file, "%s,%s,%.2f,%.2f,%.2f,%.2f,%.4f\n",
            scenario, m.algorithmName,
            m.avgWaitingTime, m.avgTurnaroundTime,
            m.avgResponseTime, m.cpuUtilization, m.fairnessIndex);
}

/* ── Run all 6 algorithms on one scenario ────────────────────────────── */
void runScenario(Process processes[], int count,
                 int scenario, char *scenarioName, FILE *file) {

    printf("\n╔══════════════════════════════════════════════╗\n");
    printf("║  %-44s║\n", scenarioName);
    printf("╚══════════════════════════════════════════════╝\n");

    printWorkload(processes, count);

    /* ── Run all algorithms ── */
    Metrics f        = fcfs(processes, count, scenario);
    Metrics s        = sjf(processes, count, scenario);
    Metrics rr2      = roundRobin(processes, count, 2, scenario);
    Metrics rr4      = roundRobin(processes, count, 4, scenario);
    Metrics rr8      = roundRobin(processes, count, 8, scenario);
    Metrics pri      = priorityScheduling(processes, count, scenario);
    Metrics priAging = priorityWithAging(processes, count, scenario);
    Metrics cfsSched = cfs(processes, count, scenario);
    Metrics lottery  = lotteryScheduling(processes, count, scenario);

    /* ── Print to terminal ── */
    printTableHeader();
    printRow(f);
    printRow(s);
    printRow(rr2);
    printRow(rr4);
    printRow(rr8);
    printRow(pri);
    printRow(priAging);
    printRow(cfsSched);
    printRow(lottery);
    printf("──────────────────────────────────────────────────────────────────────────\n");

    /* ── Save to CSV ── */
    saveRow(file, f,        scenarioName);
    saveRow(file, s,        scenarioName);
    saveRow(file, rr2,      scenarioName);
    saveRow(file, rr4,      scenarioName);
    saveRow(file, rr8,      scenarioName);
    saveRow(file, pri,      scenarioName);
    saveRow(file, priAging, scenarioName);
    saveRow(file, cfsSched, scenarioName);
    saveRow(file, lottery,  scenarioName);
}

/* ── MAIN ────────────────────────────────────────────────────────────── */
int main() {
    Process processes[MAX_PROCESSES];
    int count;

    FILE *file = fopen("results/results.csv", "w");
    if (file == NULL) {
        printf("ERROR: Could not open results/results.csv\n");
        printf("Make sure the results/ folder exists!\n");
        return 1;
    }

    fprintf(file, "Scenario,Algorithm,Avg_Wait,Avg_Turnaround,"
                  "Avg_Response,CPU_Util,Fairness_Index\n");

    printf("\n╔══════════════════════════════════════════════╗\n");
    printf("║     CSC308 - Fairness in CPU Scheduling     ║\n");
    printf("║            Simulation Results               ║\n");
    printf("╚══════════════════════════════════════════════╝\n");

    count = generateWorkload(processes, SCENARIO_A);
    runScenario(processes, count, SCENARIO_A,
                "Scenario A - Mixed Workload", file);

    count = generateWorkload(processes, SCENARIO_B);
    runScenario(processes, count, SCENARIO_B,
                "Scenario B - Batch Only", file);

    count = generateWorkload(processes, SCENARIO_C);
    runScenario(processes, count, SCENARIO_C,
                "Scenario C - RT and Interactive", file);

    count = generateWorkload(processes, SCENARIO_D);
    runScenario(processes, count, SCENARIO_D,
                "Scenario D - Balanced", file);

    fclose(file);

    printf("\n✅ All simulations complete!\n");
    printf("📁 Results saved to: results/results.csv\n");
    printf("   Open in Excel or Google Sheets to make graphs.\n\n");

    return 0;
}
