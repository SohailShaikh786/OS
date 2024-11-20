#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

typedef struct {
    char pid[5];         // Process ID
    int arrival_time;    // Arrival Time
    int burst_time;      // Burst Time
    int completion_time; // Completion Time
    int turnaround_time; // Turnaround Time
    int waiting_time;    // Waiting Time
    int response_time;   // Response Time
    bool completed;      // Process Completion Status
} Process;

// Function to sort processes based on arrival time, and burst time for ties
void sort_by_arrival_and_burst(Process processes[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (processes[j].arrival_time > processes[j + 1].arrival_time ||
                (processes[j].arrival_time == processes[j + 1].arrival_time &&
                 processes[j].burst_time > processes[j + 1].burst_time)) {
                Process temp = processes[j];
                processes[j] = processes[j + 1];
                processes[j + 1] = temp;
            }
        }
    }
}

// Function to find the next process to execute
int find_next_process(Process processes[], int n, int current_time) {
    int shortest_index = -1;
    int shortest_burst = INT_MAX;

    for (int i = 0; i < n; i++) {
        if (!processes[i].completed && processes[i].arrival_time <= current_time) {
            if (processes[i].burst_time < shortest_burst) {
                shortest_burst = processes[i].burst_time;
                shortest_index = i;
            }
        }
    }

    return shortest_index;
}

// Function to perform SJF Non-Preemptive Scheduling
void sjf_non_preemptive(Process processes[], int n) {
    sort_by_arrival_and_burst(processes, n);

    int current_time = 0;
    int completed_processes = 0;

    float total_turnaround_time = 0;
    float total_waiting_time = 0;

    printf("Gantt Chart:\n");
    while (completed_processes < n) {
        int next_process = find_next_process(processes, n, current_time);

        if (next_process != -1) {
            Process *current = &processes[next_process];

            if (current_time < current->arrival_time) {
                current_time = current->arrival_time; // Advance time if CPU is idle
            }

            printf("| %s ", current->pid);
            int start_time = current_time;
            current_time += current->burst_time;
            int end_time = current_time;

            // Update process metrics
            current->completion_time = end_time;
            current->turnaround_time = current->completion_time - current->arrival_time;
            current->waiting_time = current->turnaround_time - current->burst_time;
            current->response_time = start_time - current->arrival_time;
            current->completed = true;

            total_turnaround_time += current->turnaround_time;
            total_waiting_time += current->waiting_time;

            completed_processes++;
        } else {
            current_time++; // No process ready, increment time
        }
    }
    printf("|\n");

    // Print results
    printf("\nPID\tArrival\tBurst\tCompletion\tTurnaround\tWaiting\tResponse\n");
    for (int i = 0; i < n; i++) {
        printf("%s\t%d\t%d\t%d\t\t%d\t\t%d\t%d\n",
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
}

int main() {
    int n;
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    Process processes[n];
    for (int i = 0; i < n; i++) {
        printf("Enter Process ID, Arrival Time, and Burst Time for Process %d: ", i + 1);
        scanf("%s %d %d", processes[i].pid, &processes[i].arrival_time, &processes[i].burst_time);
        processes[i].completed = false;
    }

    sjf_non_preemptive(processes, n);

    return 0;
}
