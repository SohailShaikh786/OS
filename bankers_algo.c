#include <stdio.h>
#include <stdbool.h>

#define MAX_PROCESSES 5
#define MAX_RESOURCES 3
// Function to check if the system is in a safe state
bool isSafe(int processes[], int available[], int max[][MAX_RESOURCES], int allocation[][MAX_RESOURCES], int need[][MAX_RESOURCES]) {
    int work[MAX_RESOURCES];
    bool finish[MAX_PROCESSES] = {false};
    int safeSequence[MAX_PROCESSES];
    int count = 0;

    // Copy the available resources to work
    for (int i = 0; i < MAX_RESOURCES; i++) {
        work[i] = available[i];
    }

    // Find a process that can be safely executed
    while (count < MAX_PROCESSES) {
        bool found = false;
        for (int i = 0; i < MAX_PROCESSES; i++) {
            if (!finish[i]) {
                int j;
                // Check if the process can be executed with available resources
                for (j = 0; j < MAX_RESOURCES; j++) {
                    if (need[i][j] > work[j]) {
                        break;
                    }
                }

                // If all needs can be satisfied
                if (j == MAX_RESOURCES) {
                    for (int k = 0; k < MAX_RESOURCES; k++) {
                        work[k] += allocation[i][k];
                    }
                    safeSequence[count++] = processes[i];
                    finish[i] = true;
                    found = true;
                }
            }
        }

        // If no process was found, system is not in a safe state
        if (!found) {
            printf("System is not in a safe state!\n");
            return false;
        }
    }

    // System is in a safe state
    printf("System is in a safe state.\nSafe sequence is: ");
    for (int i = 0; i < MAX_PROCESSES; i++) {
        printf("%d ", safeSequence[i]);
    }
    printf("\n");
    return true;
}

// Function to request resources for a process
bool requestResources(int processes[], int available[], int max[][MAX_RESOURCES], int allocation[][MAX_RESOURCES], int need[][MAX_RESOURCES], int process_id, int request[]) {
    // Check if the request is greater than need
    for (int i = 0; i < MAX_RESOURCES; i++) {
        if (request[i] > need[process_id][i]) {
            printf("Error: Process has exceeded its maximum claim.\n");
            return false;
        }
    }

    // Check if the request is greater than available resources
    for (int i = 0; i < MAX_RESOURCES; i++) {
        if (request[i] > available[i]) {
            printf("Resources are not available at the moment.\n");
            return false;
        }
    }

    // Pretend to allocate requested resources
    for (int i = 0; i < MAX_RESOURCES; i++) {
        available[i] -= request[i];
        allocation[process_id][i] += request[i];
        need[process_id][i] -= request[i];
    }

    // Check if the system is in a safe state after allocation
    if (isSafe(processes, available, max, allocation, need)) {
        printf("Request granted to process %d\n", process_id);
        return true;
    } else {
        // Revert allocation if the system is not in a safe state
        for (int i = 0; i < MAX_RESOURCES; i++) {
            available[i] += request[i];
            allocation[process_id][i] -= request[i];
            need[process_id][i] += request[i];
        }
        printf("Request cannot be granted as it leads to an unsafe state.\n");
        return false;
    }
}

int main() {
    // Number of processes and resources
    int processes[MAX_PROCESSES] = {0, 1, 2, 3, 4};
    
    // Available resources
    int available[MAX_RESOURCES] = {3, 3, 2};

    // Maximum demand of each process
    int max[MAX_PROCESSES][MAX_RESOURCES] = {
        {7, 5, 3},
        {3, 2, 2},
        {9, 0, 2},
        {2, 2, 2},
        {4, 3, 3}
    };

    // Currently allocated resources
    int allocation[MAX_PROCESSES][MAX_RESOURCES] = {
        {0, 1, 0},
        {2, 0, 0},
        {3, 0, 2},
        {2, 1, 1},
        {0, 0, 2}
    };

    // Calculate the need matrix
    int need[MAX_PROCESSES][MAX_RESOURCES];
    for (int i = 0; i < MAX_PROCESSES; i++) {
        for (int j = 0; j < MAX_RESOURCES; j++) {
            need[i][j] = max[i][j] - allocation[i][j];
        }
    }

    // Check the initial state of the system
    isSafe(processes, available, max, allocation, need);

    // Example resource request (for process 1)
    int request[MAX_RESOURCES] = {1, 0, 2};
    int process_id = 1;
    requestResources(processes, available, max, allocation, need, process_id, request);

    return 0;
}
