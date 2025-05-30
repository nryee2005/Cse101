#include <stdio.h>
#include "preemptive_scheduling.h"

int main() {
    Process processes[] = {
	//{pid, arrival_time,burst_time, remaining_time, priority}
        {1, 0, 8, 8, 3},
        {2, 1, 2, 2, 4},
        {3, 3, 4, 4, 4},
        {4, 4, 1, 1, 5},
        {5, 5, 6, 6, 2},
        {6, 6, 5, 5, 6},
        {7, 10, 1, 1, 1},
        {8, 12, 1, 1, 10},
        {9, 18, 5, 5, 8},
        {10, 18, 3, 3, 8}
    };
    int n = sizeof(processes) / sizeof(processes[0]);

    int snapshot_times[] = {1, 4, 6, 12, 13 };
    int snapshot_count = sizeof(snapshot_times) / sizeof(snapshot_times[0]);

    simulate_preemptive_priority_scheduling(processes, n, snapshot_times, snapshot_count);
    return 0;
}
