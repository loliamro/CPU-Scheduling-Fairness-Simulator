#ifndef SCHEDULER_PRIORITY_H
#define SCHEDULER_PRIORITY_H

#include "process.h"
#include "metrics.h"

// Function declarations
Metrics priorityScheduling(Process processes[], int count, int scenario);
Metrics priorityWithAging(Process processes[], int count, int scenario);

#endif