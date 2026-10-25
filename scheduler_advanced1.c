/*
 * scheduler_advanced1.c
 * Algorithm #5: Completely Fair Scheduler (CFS)
 * CSC308 - Person 3: Mohammed Fares Al Korani
 */

#include <stdio.h>
#include <string.h>
#include "process.h"
#include "metrics.h"
#include "scheduler_advanced1.h"

#define NICE0_WEIGHT    1024
#define TARGET_LATENCY    20
#define MIN_GRANULARITY    4

/* priority 1=highest, 10=lowest → weight */
static const unsigned int pw[11] = {
    0, 4096, 2048, 1448, 1024, 724, 512, 362, 256, 181, 128
};

typedef struct {
    long         vruntime;
    unsigned int weight;
    int          done;
} CFSState;

Metrics cfs(Process processes[], int count, int scenario) {
    Process  local[50];
    CFSState st[50];

    for (int i = 0; i < count; i++) {
        local[i]                 = processes[i];
        local[i].remaining_time  = processes[i].burst_time;
        local[i].start_time      = -1;
        local[i].finish_time     = -1;
        local[i].waiting_time    = 0;
        local[i].turnaround_time = 0;
        local[i].response_time   = 0;
        int p = processes[i].priority;
        if (p < 1) p = 1; if (p > 10) p = 10;
        st[i].vruntime = 0;
        st[i].weight   = pw[p];
        st[i].done     = 0;
    }

    int time = 0, completed = 0;

    while (completed < count) {
        /* total weight of arrived, unfinished procs */
        unsigned int rqw = 0;
        for (int i = 0; i < count; i++)
            if (!st[i].done && local[i].arrival_time <= time)
                rqw += st[i].weight;

        /* pick lowest vruntime */
        int sel = -1; long minvr = 0x7FFFFFFF;
        for (int i = 0; i < count; i++)
            if (!st[i].done && local[i].arrival_time <= time
                && st[i].vruntime < minvr) {
                minvr = st[i].vruntime; sel = i;
            }

        if (sel == -1) {
            int next = 99999;
            for (int i = 0; i < count; i++)
                if (!st[i].done && local[i].arrival_time < next)
                    next = local[i].arrival_time;
            time = next; continue;
        }

        /* time slice proportional to weight */
        int slice = (rqw > 0)
            ? (int)((double)TARGET_LATENCY * st[sel].weight / rqw)
            : MIN_GRANULARITY;
        if (slice < MIN_GRANULARITY) slice = MIN_GRANULARITY;
        if (slice > local[sel].remaining_time)
            slice = local[sel].remaining_time;

        if (local[sel].start_time == -1) {
            local[sel].start_time   = time;
            local[sel].response_time = time - local[sel].arrival_time;
        }

        /* accumulate waiting for others */
        for (int i = 0; i < count; i++)
            if (i != sel && !st[i].done && local[i].arrival_time <= time)
                local[i].waiting_time += slice;

        time += slice;
        local[sel].remaining_time -= slice;
        st[sel].vruntime += (long)slice * NICE0_WEIGHT / st[sel].weight;

        if (local[sel].remaining_time == 0) {
            local[sel].finish_time     = time;
            local[sel].turnaround_time = time - local[sel].arrival_time;
            if (local[sel].waiting_time < 0) local[sel].waiting_time = 0;
            st[sel].done = 1;
            completed++;
        }
    }

    return calculateMetrics(local, count, "CFS", time);
}
