#ifndef PREEMPTIVE_SCHEDULING_H
#define PREEMPTIVE_SCHEDULING_H

#define MAX_TIME 1000

/**
 * @brief Structure representing a process in the scheduling simulation.
 * 
 * Each process contains its process ID, arrival time, burst time, remaining execution time, and priority.
 */
typedef struct {
    int pid;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int priority;
} Process;

/**
 * @brief Simulates preemptive priority scheduling for a set of processes.
 * 
 * The function executes the scheduling simulation, printing snapshots of ready queue and gantt charts
 * at given times and building the corresponding Gantt chart.
 * 
 * @param processes Array of processes to schedule
 * @param n:  Number of processes
 * @param snapshot_times Array of times at which to print the ready queue and gantt charts snapshot
 * @param snapshot_count Number of snapshot times provided
 */
void simulate_preemptive_priority_scheduling(Process processes[], int n, int snapshot_times[], int snapshot_count);

/**
 * @brief Prints the current ready queue of processes.
 * 
 * This function displays the ready queue at a given simulation time, showing the processes waiting for CPU time.
 */
void print_ready_queue();

/**
 * @brief Prints the Gantt chart up to a certain execution index.
 * 
 * The Gantt chart visually represents the execution order and duration of processes.
 * 
 * @param upto_index Index up to which the Gantt chart should be printed.
 */
void print_gantt_chart(int upto_index);

/**
 * @brief Comparator function used to compare two processes based on priority.
 * 
 * This function determines the ordering of processes by comparing their priorities,
 * and is used by the priority queue during scheduling.
 * 
 * @param a Pointer to the first process
 * @param b Pointer to the second process
 * @return int Negative if a has higher priority, positive if b has higher priority, zero if equal
 */
int process_comparator(void *a, void *b);


#endif
