#ifndef SCHEDULER_ADVANCED1_H
#define SCHEDULER_ADVANCED1_H

#include "process.h"
#include "metrics.h"

// Algorithm 5: Completely Fair Scheduler (CFS)
Metrics cfs(Process processes[], int count, int scenario);

#endif
