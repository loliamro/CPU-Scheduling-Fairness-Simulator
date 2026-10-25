#ifndef SCHEDULER_FCFS_H
#define SCHEDULER_FCFS_H

#include "process.h"
#include "metrics.h"

Metrics fcfs(Process processes[], int count, int scenario);

#endif