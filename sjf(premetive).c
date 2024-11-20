#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

#define MAX_PROCESSES 100

typedef struct {
    int pid;            // Process ID
    int arrival_time;   // Arrival Time
    int burst_time;     // Burst Time
    int remaining_time; // Remaining Burst Time
    int completion_time; // Completion Time
    int turnaround_time; // Turnaround Time
    int waiting_time;    // Waiting Time
    int response_time;   // Response Time
    bool started;        // Check if the process has started
} Process;

// Function to find the process with the shortest remaining time
int find_shortest_job(Process processes[], int n, int current_time) {
    int shortest_index = -1;
    int shortest_time = INT_MAX;

    for (int i = 0; i < n; i++) {
        if (processes[i].arrival_time <= current_time && processes[i].remaining_time > 0) {
            if (processes[i].remaining_time < shortest_time) {
                shortest_time = processes[i].remaining_time;
                shortest_index = i;
            }
        }
    }

    return shortest_index;
}

// Function to print the Gantt chart
void print_gantt_chart(int gantt_chart[][2], int size) {
    printf("\nGantt Chart:\n");

    // Print process IDs
    for (int i = 0; i < size; i++) {
        printf("| P%d ", gantt_chart[i][0]);
    }
    printf("|\n");

    // Print timeline
    for (int i = 0; i < size; i++) {
        printf("%d    ", gantt_chart[i][1]);
    }
    printf("%d\n", gantt_chart[size - 1][1] + 1);
}

// Function to perform SJF Preemptive Scheduling
void shortest_job_first_preemptive(Process processes[], int n) {
    int current_time = 0;
    int completed = 0;
    int gantt_chart[MAX_PROCESSES][2]; // Gantt Chart: Process ID and start time
    int gantt_index = 0;

    float total_waiting_time = 0; // Total waiting time
    float total_turnaround_time = 0; // Total turnaround time

    while (completed < n) {
        // Find the shortest remaining time process
        int shortest_index = find_shortest_job(processes, n, current_time);

        if (shortest_index != -1) {
            Process *current_process = &processes[shortest_index];

            // Record the Gantt chart only when a new process starts
            if (gantt_index == 0 || gantt_chart[gantt_index - 1][0] != current_process->pid) {
                gantt_chart[gantt_index][0] = current_process->pid;
                gantt_chart[gantt_index][1] = current_time;
                gantt_index++;
            }

            // Set response time for the first time the process starts
            if (!current_process->started) {
                current_process->response_time = current_time - current_process->arrival_time;
                current_process->started = true;
            }

            // Execute the process for 1 time unit
            current_process->remaining_time--;
            current_time++;

            // If the process is completed
            if (current_process->remaining_time == 0) {
                current_process->completion_time = current_time;
                current_process->turnaround_time = current_process->completion_time - current_process->arrival_time;
                current_process->waiting_time = current_process->turnaround_time - current_process->burst_time;

                // Update totals
                total_waiting_time += current_process->waiting_time;
                total_turnaround_time += current_process->turnaround_time;

                completed++;
            }
        } else {
            // No process is ready to execute, increment time
            current_time++;
        }
    }

    // Print results
    printf("PID\tArrival Time\tBurst Time\tCompletion Time\tTurnaround Time\tWaiting Time\tResponse Time\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n",
               processes[i].pid,
               processes[i].arrival_time,
               processes[i].burst_time,
               processes[i].completion_time,
               processes[i].turnaround_time,
               processes[i].waiting_time,
               processes[i].response_time);
    }

    // Print averages
    printf("\nAverage Turnaround Time: %.2f\n", total_turnaround_time / n);
    printf("Average Waiting Time: %.2f\n", total_waiting_time / n);

    // Print Gantt Chart
    print_gantt_chart(gantt_chart, gantt_index);
}

// Main function
int main() {
    Process processes[MAX_PROCESSES];
    int n;

    printf("Enter the number of processes: ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        printf("Enter Arrival Time and Burst Time for Process %d: ", i + 1);
        processes[i].pid = i + 1;
        scanf("%d %d", &processes[i].arrival_time, &processes[i].burst_time);
        processes[i].remaining_time = processes[i].burst_time;
        processes[i].started = false;
    }

    shortest_job_first_preemptive(processes, n);

    return 0;
}
