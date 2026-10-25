#ifndef SCHEDULER_RR_H
#define SCHEDULER_RR_H

#include "process.h"
#include "metrics.h"

// Function declarations
Metrics roundRobin(Process processes[], int count, int quantum, int scenario);

#endif