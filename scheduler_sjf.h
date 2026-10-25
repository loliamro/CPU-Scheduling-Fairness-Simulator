#ifndef SCHEDULER_SJF_H
#define SCHEDULER_SJF_H

#include "process.h"
#include "metrics.h"

Metrics sjf(Process processes[], int count, int scenario);

#endif