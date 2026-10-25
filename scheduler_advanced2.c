/*
 * scheduler_advanced2.c
 * Algorithm #6: Lottery Scheduling
 * CSC308 - Person 3: Mohammed Fares Al Korani
 *
 * HOW LOTTERY WORKS (Waldspurger & Weihl, 1994):
 *   Each process is given lottery tickets proportional to its priority.
 *   At each scheduling decision, a random ticket is drawn.
 *   The process holding that ticket wins the CPU for one time quantum.
 *   Over time, each process gets CPU proportional to its ticket share.
 *   This gives probabilistic fairness — fair over the long run.
 *
 * TICKETS:
 *   priority 1 (highest) → 10 tickets
 *   priority 5 (medium)  →  6 tickets
 *   priority 10 (lowest) →  1 ticket
 *   Formula: tickets = 11 - priority
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "process.h"
#include "metrics.h"
#include "scheduler_advanced2.h"

#define LOTTERY_QUANTUM  10   /* time slice given to winner (ms) */
#define TICKET_BASE      11   /* tickets = TICKET_BASE - priority */

/* Map priority → ticket count */
static int tickets_for(int priority) {
    if (priority < 1)  priority = 1;
    if (priority > 10) priority = 10;
    return TICKET_BASE - priority;   /* priority 1 → 10 tickets, 10 → 1 */
}

Metrics lotteryScheduling(Process processes[], int count, int scenario) {

    /* Step 1: Make a local copy */
    Process local[50];
    int     done[50]    = {0};
    int     tickets[50];         /* each process's ticket count  */

    for (int i = 0; i < count; i++) {
        local[i]                 = processes[i];
        local[i].remaining_time  = processes[i].burst_time;
        local[i].start_time      = -1;
        local[i].finish_time     = -1;
        local[i].waiting_time    = 0;
        local[i].turnaround_time = 0;
        local[i].response_time   = 0;
        tickets[i] = tickets_for(processes[i].priority);
    }

    /* Fixed seed → same results every run (important for reproducibility) */
    srand(42);

    int time      = 0;
    int completed = 0;

    /* Step 2: Main scheduling loop */
    while (completed < count) {

        /* Count total tickets among arrived, unfinished processes */
        int total_tickets = 0;
        for (int i = 0; i < count; i++)
            if (!done[i] && local[i].arrival_time <= time)
                total_tickets += tickets[i];

        /* No process ready — advance to next arrival */
        if (total_tickets == 0) {
            int next = 99999;
            for (int i = 0; i < count; i++)
                if (!done[i] && local[i].arrival_time < next)
                    next = local[i].arrival_time;
            time = next;
            continue;
        }

        /*
         * Draw a winning ticket: 0 .. total_tickets-1
         * Walk through arrived processes until we find the winner.
         */
        int winning = rand() % total_tickets;
        int sel     = -1;
        int running = 0;

        for (int i = 0; i < count; i++) {
            if (!done[i] && local[i].arrival_time <= time) {
                running += tickets[i];
                if (winning < running) { sel = i; break; }
            }
        }

        if (sel == -1) continue;  /* safety guard */

        /* First time on CPU? */
        if (local[sel].start_time == -1) {
            local[sel].start_time   = time;
            local[sel].response_time = time - local[sel].arrival_time;
        }

        /* Run for quantum or until process finishes */
        int run = (local[sel].remaining_time < LOTTERY_QUANTUM)
                ? local[sel].remaining_time
                : LOTTERY_QUANTUM;

        /* Accumulate waiting time for all OTHER arrived, unfinished procs */
        for (int i = 0; i < count; i++)
            if (i != sel && !done[i] && local[i].arrival_time <= time)
                local[i].waiting_time += run;

        time                      += run;
        local[sel].remaining_time -= run;

        /* Check completion */
        if (local[sel].remaining_time == 0) {
            local[sel].finish_time     = time;
            local[sel].turnaround_time = time - local[sel].arrival_time;
            if (local[sel].waiting_time < 0) local[sel].waiting_time = 0;
            done[sel] = 1;
            completed++;
        }
    }

    return calculateMetrics(local, count, "Lottery Scheduling", time);
}
