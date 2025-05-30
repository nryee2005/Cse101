#include <stdio.h>
#include "preemptive_scheduling.h"
#include "priority_queue.h"
#include <stdlib.h>
#include <string.h>

// Arrays to store Gantt charts for both CPUs
char gantt_chart_cpu0[MAX_TIME][10];
char gantt_chart_cpu1[MAX_TIME][10];
int time_chart[MAX_TIME]; // Tracks the time slots

// Process comparator function for priority scheduling
int process_comparator(void *a, void *b) {
    // compare priority 
    int diff = ((Process *)a)->priority - ((Process *)b)->priority;

    // if priorities are equal, compare arrival times
    if (diff==0) {
        return ((Process *)a)->arrival_time - ((Process *)b)->arrival_time;
    }

    return diff;
}

// Priority queue for ready queue
PriorityQueue ready_queue;

// Function to print the ready queue
void print_ready_queue() {
    printf("Ready Queue:\n");
    print_queue(&ready_queue);
    printf("\n");
}

// Function to print the Gantt chart for both CPUs
void print_gantt_chart(int upto_index) {
    // Print the Gantt chart
    printf("Gantt Chart for CPU 0:\n");
    for (int i = 0; i <= upto_index; i++) {
        printf("| %s", gantt_chart_cpu0[i]);
        if (strcmp(gantt_chart_cpu0[i], "IDLE") != 0) {
            printf("  ");
        }
    }
    printf("\n");
    for (int i = 0; i <= upto_index; i++) {
        printf("%d    ", i);
        if (i < 10) {
            printf(" ");
        }
    }
    printf("\n");
    printf("Gantt Chart for CPU 1:\n");
    for (int i = 0; i <= upto_index; i++) {
        printf("| %s", gantt_chart_cpu1[i]);
        if (strcmp(gantt_chart_cpu1[i], "IDLE") != 0) {
            printf("  ");
        }
    }
    printf("\n");
    for (int i = 0; i <= upto_index; i++) {
        printf("%d    ", i);
        if (i < 10) {
            printf(" ");
        }
    }
    printf("\n");
}

// The main simulation function for preemptive priority scheduling
void simulate_preemptive_priority_scheduling(Process processes[], int n, int snapshot_times[], int snapshot_count) {
    // Check for invalid input
    if (n <= 0) {
        printf("Error: Number of processes must be greater than 0\n");
        return;
    }

    for (int i = 0; i < n; i++) {
        if (processes[i].arrival_time < 0) {
            printf("Error: Arrival time cannot be negative\n");
            return;
        }

        if (processes[i].burst_time <= 0) {
            printf("Error: Burst time must be greater than 0\n");
            return;
        }

        if (processes[i].priority <= 0) {
            printf("Error: Priority must be greater than 0\n");
            return;
        }
    }

    pq_init(&ready_queue, process_comparator);
    int cc = 0; // Current clock cycle

    while (cc < MAX_TIME) {

        // Insert processes with the same arrival time into the ready queue
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time == cc) {
                pq_insert(&ready_queue, &processes[i]);
            }
        }
        
        // Check if the ready queue is empty
        if (pq_is_empty(&ready_queue)) {
            snprintf(gantt_chart_cpu0[cc], 10, "IDLE");
            snprintf(gantt_chart_cpu1[cc], 10, "IDLE");
        }
        else {
            // Execute the process with the highest priority
            Process *p = (Process *)ready_queue.data[0];
            p->remaining_time--;
            if (p->remaining_time == 0) {
                pq_delete(&ready_queue);
            }
            // Update the Gantt chart
            snprintf(gantt_chart_cpu0[cc], 10, "P%d", (p->pid));
        }
        if (ready_queue.size > 1) {
            // Execute the process with the second highest priority
            Process *p1 = (Process *)ready_queue.data[1];
            p1->remaining_time--;
            if (p1->remaining_time == 0) {
                Process *p0 = pq_delete(&ready_queue);
                pq_delete(&ready_queue);
                pq_insert(&ready_queue, p0);
            }
            // Update the Gantt chart
            snprintf(gantt_chart_cpu1[cc], 10, "P%d", (p1->pid));
        }
        else {
            // If there is only one process in the ready queue, set it to idle
            snprintf(gantt_chart_cpu1[cc], 10, "IDLE");
        }

        // Check if all processes have been executed
        int count = 0;
        for (int i = 0; i<n; i++) {
            if (processes[i].remaining_time != 0) {
                count++;
            }   
        }
        // If all processes have been executed, print the final Gantt chart
        if (count == 0) {
            printf("-------------------------------------------------------\n");
            printf("Final Gantt Chart after all the processes are executed:\n");
            printf("-------------------------------------------------------\n");
            print_gantt_chart(cc);
            break;
        }

        // Check if we need to print a snapshot
        for (int i = 0; i<snapshot_count; i++) {
            if (cc == snapshot_times[i]) {
                printf("------------------------------------\n");
                printf("Snapshot at clock time %d:\n", cc);
                printf("------------------------------------\n");
                print_gantt_chart(cc);
                print_ready_queue();
            }
        }
        
        // Increment the clock
        cc++;
    }
    
}
