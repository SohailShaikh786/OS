#include <stdio.h>
#include <stdbool.h>

#define MAX_PROCESSES 100

typedef struct {
    int pid;             // Process ID
    int arrival_time;    // Arrival Time
    int burst_time;      // Burst Time
    int remaining_time;  // Remaining Burst Time
    int completion_time; // Completion Time
    int turnaround_time; // Turnaround Time
    int waiting_time;    // Waiting Time
    int response_time;   // Response Time
    bool started;        // Indicates if the process has started
} Process;

void round_robin(Process processes[], int n, int time_quantum) {
    int time = 0;              // Current time in the system
    int completed = 0;         // Number of completed processes
    int queue[MAX_PROCESSES];  // Ready queue
    int front = 0, rear = 0;   // Queue pointers for maintaining the queue
    float total_waiting_time = 0; // Total waiting time
    float total_turnaround_time = 0; // Total turnaround time

    int gantt_chart[MAX_PROCESSES * 2][2]; // Gantt Chart: Process ID and start time
    int gantt_index = 0;

    // Initialize the ready queue
    for (int i = 0; i < n; i++) {
        processes[i].remaining_time = processes[i].burst_time;
        processes[i].started = false;
    }

    while (completed < n) {
        // Add new arrivals to the queue
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= time && processes[i].remaining_time > 0) {
                bool in_queue = false;
                for (int j = front; j < rear; j++) {
                    if (queue[j] == i) {
                        in_queue = true;
                        break;
                    }
                }
                if (!in_queue) {
                    queue[rear++] = i;
                }
            }
        }

        if (front < rear) {
            int current = queue[front++]; // Dequeue the process

            // Record the Gantt chart
            if (gantt_index == 0 || gantt_chart[gantt_index - 1][0] != processes[current].pid) {
                gantt_chart[gantt_index][0] = processes[current].pid;
                gantt_chart[gantt_index][1] = time;
                gantt_index++;
            }

            // Record response time for the first execution
            if (!processes[current].started) {
                processes[current].response_time = time - processes[current].arrival_time;
                processes[current].started = true;
            }

            // Execute the process for time_quantum or remaining time
            if (processes[current].remaining_time > time_quantum) {
                time += time_quantum;
                processes[current].remaining_time -= time_quantum;
                queue[rear++] = current; // Re-enqueue the process
            } else {
                time += processes[current].remaining_time;
                processes[current].remaining_time = 0;
                processes[current].completion_time = time;
                processes[current].turnaround_time = processes[current].completion_time - processes[current].arrival_time;
                processes[current].waiting_time = processes[current].turnaround_time - processes[current].burst_time;

                // Update totals
                total_waiting_time += processes[current].waiting_time;
                total_turnaround_time += processes[current].turnaround_time;

                completed++;
            }
        } else {
            time++; // Increment time if no process is ready
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
    printf("\nGantt Chart:\n");
    for (int i = 0; i < gantt_index; i++) {
        printf("| P%d ", gantt_chart[i][0]);
    }
    printf("|\n");
    for (int i = 0; i < gantt_index; i++) {
        printf("%d    ", gantt_chart[i][1]);
    }
    printf("%d\n", time);
}

int main() {
    int n, time_quantum;

    // Input number of processes and time quantum
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    Process processes[MAX_PROCESSES];
    for (int i = 0; i < n; i++) {
        processes[i].pid = i + 1;
        printf("Enter Arrival Time and Burst Time for Process %d: ", i + 1);
        scanf("%d %d", &processes[i].arrival_time, &processes[i].burst_time);
    }

    printf("Enter Time Quantum: ");
    scanf("%d", &time_quantum);

    // Call Round Robin function
    round_robin(processes, n, time_quantum);

    return 0;
}
