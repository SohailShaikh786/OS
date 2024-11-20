#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX 100

typedef struct {
    char name[5];
    int burst_time;
    int arrival_time;
    int priority;
    int waiting_time;
    int turnaround_time;
    int response_time;
    int completion_time;
    bool is_completed;
} Process;

void priority_scheduling(Process processes[], int n) {
    int time = 0, completed = 0;
    float total_waiting_time = 0, total_turnaround_time = 0;
    int gantt_chart[MAX], gantt_chart_time[MAX], gantt_index = 0;

    // Initialize process status
    for (int i = 0; i < n; i++) {
        processes[i].is_completed = false;
    }

    while (completed < n) {
        int current_process = -1;
        int highest_priority = _INT_MAX_;

        // Find the highest priority process that has arrived and is not completed
        for (int i = 0; i < n; i++) {
            if (!processes[i].is_completed && processes[i].arrival_time <= time && processes[i].priority < highest_priority) {
                highest_priority = processes[i].priority;
                current_process = i;
            }
        }

        if (current_process == -1) {
            // No process is ready; idle time
            gantt_chart[gantt_index] = -1; // Idle
            gantt_chart_time[gantt_index++] = time;
            time++;
            continue;
        }

        // Execute the selected process
        gantt_chart[gantt_index] = current_process; // Track process in Gantt Chart
        gantt_chart_time[gantt_index++] = time;

        if (!processes[current_process].is_completed) {
            processes[current_process].response_time = time - processes[current_process].arrival_time;
        }

        time += processes[current_process].burst_time;
        processes[current_process].completion_time = time;
        processes[current_process].turnaround_time = time - processes[current_process].arrival_time;
        processes[current_process].waiting_time = processes[current_process].turnaround_time - processes[current_process].burst_time;
        processes[current_process].is_completed = true;

        total_waiting_time += processes[current_process].waiting_time;
        total_turnaround_time += processes[current_process].turnaround_time;

        completed++;
    }

    gantt_chart_time[gantt_index++] = time;

    // Print process details
    printf("\nProcess\tAT\tBT\tPri\tCT\tTAT\tWT\tRT\n");
    for (int i = 0; i < n; i++) {
        printf("%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
               processes[i].name, processes[i].arrival_time, processes[i].burst_time,
               processes[i].priority, processes[i].completion_time, processes[i].turnaround_time,
               processes[i].waiting_time, processes[i].response_time);
    }

    // Print average times
    printf("\nAverage Waiting Time: %.2f\n", total_waiting_time / n);
    printf("Average Turnaround Time: %.2f\n", total_turnaround_time / n);

    // Print Gantt chart
    printf("\nGantt Chart:\n|");
    for (int i = 0; i < gantt_index - 1; i++) {
        if (gantt_chart[i] == -1) {
            printf(" Idle |");
        } else {
            printf(" %s |", processes[gantt_chart[i]].name);
        }
    }
    printf("\n");

    for (int i = 0; i < gantt_index; i++) {
        printf("%d\t", gantt_chart_time[i]);
    }
    printf("\n");
}

int main() {
    int n;

    printf("Enter the number of processes: ");
    scanf("%d", &n);

    Process processes[MAX];
    for (int i = 0; i < n; i++) {
        printf("Enter process name, arrival time, burst time, and priority for process %d: ", i + 1);
        scanf("%s %d %d %d", processes[i].name, &processes[i].arrival_time, &processes[i].burst_time, &processes[i].priority);
    }

    priority_scheduling(processes, n);

    return 0;
}
