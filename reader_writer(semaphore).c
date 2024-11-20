#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Constants
#define MAX_OPERATIONS 20 // Total read/write operations

// Semaphores and shared data
sem_t rw_mutex;      // Semaphore to ensure mutual exclusion for writers
sem_t mutex;         // Semaphore to protect read_count
int read_count = 0;  // Number of readers currently accessing the resource
int shared_data = 0; // Shared resource
int total_operations = 0; // Tracks the total number of operations

// Function prototypes
void* writer(void* arg);
void* reader(void* arg);

pthread_mutex_t op_mutex = PTHREAD_MUTEX_INITIALIZER; // Protects total_operations

// Writer function
void* writer(void* arg) {
    int writer_id = *(int*)arg; // Get writer ID

    while (1) {
        // Check if the maximum number of operations has been reached
        pthread_mutex_lock(&op_mutex);
        if (total_operations >= MAX_OPERATIONS) {
            pthread_mutex_unlock(&op_mutex);
            break;
        }
        total_operations++;
        pthread_mutex_unlock(&op_mutex);

        // Writer locks the resource for writing
        sem_wait(&rw_mutex);

        // Write to the shared resource
        shared_data += 1;
        printf("Writer %d wrote: %d\n", writer_id, shared_data);

        // Release the resource
        sem_post(&rw_mutex);

        // Simulate writing time
        sleep(rand() % 2 + 1);
    }

    return NULL;
}

// Reader function
void* reader(void* arg) {
    int reader_id = *(int*)arg; // Get reader ID

    while (1) {
        // Check if the maximum number of operations has been reached
        pthread_mutex_lock(&op_mutex);
        if (total_operations >= MAX_OPERATIONS) {
            pthread_mutex_unlock(&op_mutex);
            break;
        }
        total_operations++;
        pthread_mutex_unlock(&op_mutex);

        // Entry Section: Update the reader count
        sem_wait(&mutex);
        read_count++;
        if (read_count == 1) {
            sem_wait(&rw_mutex); // First reader locks the resource
        }
        sem_post(&mutex);

        // Read from the shared resource
        printf("Reader %d read: %d\n", reader_id, shared_data);

        // Exit Section: Update the reader count
        sem_wait(&mutex);
        read_count--;
        if (read_count == 0) {
            sem_post(&rw_mutex); // Last reader releases the resource
        }
        sem_post(&mutex);

        // Simulate reading time
        sleep(rand() % 2 + 1);
    }

    return NULL;
}

// Main function
int main() {
    pthread_t readers[3], writers[2]; // Arrays for reader and writer threads
    int reader_ids[3] = {1, 2, 3};   // Reader IDs
    int writer_ids[2] = {1, 2};      // Writer IDs

    // Initialize semaphores
    sem_init(&rw_mutex, 0, 1); // Binary semaphore for writers
    sem_init(&mutex, 0, 1);    // Binary semaphore for reader count

    // Create writer threads
    for (int i = 0; i < 2; i++) {
        pthread_create(&writers[i], NULL, writer, &writer_ids[i]);
    }

    // Create reader threads
    for (int i = 0; i < 3; i++) {
        pthread_create(&readers[i], NULL, reader, &reader_ids[i]);
    }

    // Join writer threads
    for (int i = 0; i < 2; i++) {
        pthread_join(writers[i], NULL);
    }

    // Join reader threads
    for (int i = 0; i < 3; i++) {
        pthread_join(readers[i], NULL);
    }

    // Destroy semaphores
    sem_destroy(&rw_mutex);
    sem_destroy(&mutex);

    printf("All operations completed. Exiting.\n");

    return 0;
}
