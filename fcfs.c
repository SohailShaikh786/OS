#include <stdio.h>
#include <string.h>

#define MAX_PROCESSES 100

typedef struct {
    char pid[10];
    int burst_time;
    int arrival_time;
} Process;

void calculate_completion_time(Process processes[], int n, int completion_times[]) {
    int current_time = 0;
    for (int i = 0; i < n; i++) {
        if (current_time < processes[i].arrival_time) {
            current_time = processes[i].arrival_time; // Wait for process to arrive
        }
        current_time += processes[i].burst_time;
        completion_times[i] = current_time;
    }
}

void calculate_turnaround_time(Process processes[], int n, int completion_times[], int turnaround_times[]) {
    for (int i = 0; i < n; i++) {
        turnaround_times[i] = completion_times[i] - processes[i].arrival_time;
    }
}

void calculate_waiting_time(Process processes[], int n, int turnaround_times[], int waiting_times[]) {
    for (int i = 0; i < n; i++) {
        waiting_times[i] = turnaround_times[i] - processes[i].burst_time;
    }
}

void calculate_response_time(Process processes[], int n, int completion_times[], int response_times[]) {
    int current_time = 0;
    for (int i = 0; i < n; i++) {
        response_times[i] = current_time - processes[i].arrival_time;
        if (current_time < processes[i].arrival_time) {
            current_time = processes[i].arrival_time;
        }
        current_time += processes[i].burst_time;
    }
}

void print_gantt_chart(Process processes[], int n) {
    printf("\nGantt Chart:\n");

    // Print top border
    for (int i = 0; i < n; i++) {
        printf(" ");
        for (int j = 0; j < processes[i].burst_time; j++) printf("-");
        printf(" ");
    }
    printf("\n|");

    // Print process IDs
    for (int i = 0; i < n; i++) {
        int mid = processes[i].burst_time / 2;
        for (int j = 0; j < processes[i].burst_time; j++) {
            if (j == mid) printf("%s", processes[i].pid);
            else printf(" ");
        }
        printf("|");
    }
    printf("\n");

    // Print bottom border
    for (int i = 0; i < n; i++) {
        printf(" ");
        for (int j = 0; j < processes[i].burst_time; j++) printf("-");
        printf(" ");
    }
    printf("\n");

    // Print timeline
    int current_time = 0;
    printf("0");
    for (int i = 0; i < n; i++) {
        if (current_time < processes[i].arrival_time) {
            current_time = processes[i].arrival_time;
        }
        current_time += processes[i].burst_time;
        printf("%*d", processes[i].burst_time + 1, current_time);
    }
    printf("\n");
}

int main() {
    Process processes[MAX_PROCESSES];
    int n;

    printf("Enter the number of processes: ");
    scanf("%d", &n);

    // Input processes
    for (int i = 0; i < n; i++) {
        printf("Enter Process ID, Arrival Time, and Burst Time for process %d: ", i + 1);
        scanf("%s %d %d", processes[i].pid, &processes[i].arrival_time, &processes[i].burst_time);
    }

    int completion_times[MAX_PROCESSES], turnaround_times[MAX_PROCESSES], waiting_times[MAX_PROCESSES], response_times[MAX_PROCESSES];

    // Calculate completion times, turnaround times, waiting times, and response times
    calculate_completion_time(processes, n, completion_times);
    calculate_turnaround_time(processes, n, completion_times, turnaround_times);
    calculate_waiting_time(processes, n, turnaround_times, waiting_times);
    calculate_response_time(processes, n, completion_times, response_times);

    // Print the results
    printf("\nProcess\tArrival Time\tBurst Time\tCompletion Time\tTurnaround Time\tWaiting Time\tResponse Time\n");
    for (int i = 0; i < n; i++) {
        printf("%s\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n",
               processes[i].pid, processes[i].arrival_time, processes[i].burst_time,
               completion_times[i], turnaround_times[i], waiting_times[i], response_times[i]);
    }

    // Calculate and display averages
    float average_turnaround_time = 0, average_waiting_time = 0, average_response_time = 0;
    for (int i = 0; i < n; i++) {
        average_turnaround_time += turnaround_times[i];
        average_waiting_time += waiting_times[i];
        average_response_time += response_times[i];
    }
    average_turnaround_time /= n;
    average_waiting_time /= n;
    average_response_time /= n;

    printf("\nAverage Turnaround Time: %.2f\n", average_turnaround_time);
    printf("Average Waiting Time: %.2f\n", average_waiting_time);
    printf("Average Response Time: %.2f\n", average_response_time);

    // Print Gantt chart
    print_gantt_chart(processes, n);

    return 0;
}
